#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <algorithm>
#include <queue>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <omp.h>

#pragma pack(push, 1)
struct PackedRecord9B {
    uint8_t  metadata_mask;
    uint16_t quantized_r;
    uint16_t quantized_theta;
    uint32_t payload_hash;
};
#pragma pack(pop)

static_assert(sizeof(PackedRecord9B) == 9, "PackedRecord9B must be strictly 9 bytes");

namespace TorkyInvariants {
    constexpr double EPSILON = 0.016;
    constexpr double V0 = 1.440;
    constexpr double KAPPA_MAX = 18.50;
    constexpr double TWO_OVER_SQRT3 = 1.1547005383792515;
    constexpr double THREE_SQRT3_EPS_OVER_8 = 0.010392304845413264;
}

class TorkyGovernanceEngine {
public:
    static double compute_finite_potential(double r) {
        return TorkyInvariants::V0 / std::sqrt((r * r) + TorkyInvariants::EPSILON);
    }

    static void reconstruct_vector(const PackedRecord9B& seed, float* output_vector_64d) {
        double r = static_cast<double>(seed.quantized_r) / 1000.0;
        double theta = (static_cast<double>(seed.quantized_theta) / 65535.0) * (2.0 * M_PI);

        double h_norm = (static_cast<double>(seed.payload_hash % 1000000ULL) / 500000.0) - 1.0;
        h_norm = std::clamp(h_norm, -1.0, 1.0);

        double sign_h = (h_norm >= 0.0) ? 1.0 : -1.0;
        double arg = -TorkyInvariants::THREE_SQRT3_EPS_OVER_8 * h_norm;
        arg = std::clamp(arg, -0.999999, 0.999999);
        double x_star = sign_h * TorkyInvariants::TWO_OVER_SQRT3 * std::cos((1.0 / 3.0) * std::acos(arg));

        for (int d = 0; d < 64; ++d) {
            double phase = theta + (static_cast<double>(d) * (2.0 * M_PI / 64.0));
            double component = (r / std::sqrt(64.0)) * std::cos(phase) * (1.0 + (0.01 * x_star));
            output_vector_64d[d] = static_cast<float>(component);
        }
    }

    static double compute_tikhonov_shift(double lambda_max, double lambda_min) {
        double abs_max = std::abs(lambda_max);
        double abs_min = std::max(std::abs(lambda_min), 1e-12);
        double current_kappa = abs_max / abs_min;
        if (current_kappa <= TorkyInvariants::KAPPA_MAX) {
            return 0.0;
        }
        return (abs_max - (TorkyInvariants::KAPPA_MAX * abs_min)) / (TorkyInvariants::KAPPA_MAX - 1.0);
    }
};

class TorkyTileStreamer {
private:
    int file_descriptor = -1;
    void* raw_mmap_addr = nullptr;
    PackedRecord9B* mapped_tile = nullptr;
    size_t total_mapped_length = 0;
    uint64_t total_capacity_records = 0;
    uint64_t records_in_current_tile = 0;
    long page_size = 4096;
    std::string file_path;

public:
    TorkyTileStreamer(const std::string& path, uint64_t total_records)
        : file_path(path), total_capacity_records(total_records) {
        page_size = sysconf(_SC_PAGE_SIZE);
        file_descriptor = open(file_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (file_descriptor < 0) {
            throw std::runtime_error("Failed to initialize physical storage backing file.");
        }

        uint64_t total_bytes = total_records * sizeof(PackedRecord9B);
        if (ftruncate(file_descriptor, total_bytes) != 0) {
            close(file_descriptor);
            throw std::runtime_error("Failed to allocate physical file capacity.");
        }
    }

    ~TorkyTileStreamer() {
        unmap_tile();
        if (file_descriptor >= 0) {
            close(file_descriptor);
        }
    }

    void map_tile(uint64_t start_record_idx, uint64_t tile_records_count) {
        unmap_tile();
        records_in_current_tile = tile_records_count;

        uint64_t raw_byte_offset = start_record_idx * sizeof(PackedRecord9B);
        uint64_t page_aligned_offset = (raw_byte_offset / page_size) * page_size;
        size_t byte_in_page = raw_byte_offset - page_aligned_offset;
        size_t required_bytes = (tile_records_count * sizeof(PackedRecord9B)) + byte_in_page;

        total_mapped_length = required_bytes;
        raw_mmap_addr = mmap(nullptr, total_mapped_length, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, page_aligned_offset);
        if (raw_mmap_addr == MAP_FAILED) {
            raw_mmap_addr = nullptr;
            throw std::runtime_error("mmap failure during tile window mapping.");
        }

        mapped_tile = reinterpret_cast<PackedRecord9B*>(static_cast<char*>(raw_mmap_addr) + byte_in_page);
    }

    void write_record(uint64_t intra_tile_offset, const PackedRecord9B& record) {
        if (intra_tile_offset < records_in_current_tile && mapped_tile != nullptr) {
            mapped_tile[intra_tile_offset] = record;
        }
    }

    PackedRecord9B* get_tile_ptr() {
        return mapped_tile;
    }

    void unmap_tile() {
        if (raw_mmap_addr != nullptr) {
            msync(raw_mmap_addr, total_mapped_length, MS_SYNC);
            munmap(raw_mmap_addr, total_mapped_length);
            raw_mmap_addr = nullptr;
            mapped_tile = nullptr;
            total_mapped_length = 0;
        }
    }
};

extern "C" {
    void* torky_create_streamer(const char* path, uint64_t total_records) {
        try {
            return new TorkyTileStreamer(std::string(path), total_records);
        } catch (...) {
            return nullptr;
        }
    }

    void torky_map_tile(void* streamer_ptr, uint64_t start_record_idx, uint64_t tile_records_count) {
        if (streamer_ptr) {
            reinterpret_cast<TorkyTileStreamer*>(streamer_ptr)->map_tile(start_record_idx, tile_records_count);
        }
    }

    void torky_write_packed(void* streamer_ptr, uint64_t intra_tile_offset, 
                            uint8_t mask, uint16_t r, uint16_t theta, uint32_t hash) {
        if (streamer_ptr) {
            PackedRecord9B rec{mask, r, theta, hash};
            reinterpret_cast<TorkyTileStreamer*>(streamer_ptr)->write_record(intra_tile_offset, rec);
        }
    }

    void torky_reconstruct_vector(uint8_t mask, uint16_t r, uint16_t theta, uint32_t hash, float* output_64d) {
        PackedRecord9B rec{mask, r, theta, hash};
        TorkyGovernanceEngine::reconstruct_vector(rec, output_64d);
    }

    // Parallel Top-K Vector Search across the currently mapped tile using OpenMP
    void torky_search_tile_topk(void* streamer_ptr, uint64_t tile_records_count, 
                                float query_theta, int k, uint64_t* out_indices, float* out_scores) {
        if (!streamer_ptr) return;
        TorkyTileStreamer* streamer = reinterpret_cast<TorkyTileStreamer*>(streamer_ptr);
        PackedRecord9B* records = streamer->get_tile_ptr();
        if (!records) return;

        // Custom comparator for min-heap
        typedef std::pair<float, uint64_t> ScoreIdx;
        std::priority_queue<ScoreIdx, std::vector<ScoreIdx>, std::greater<ScoreIdx>> topk_heap;

        #pragma omp parallel
        {
            std::priority_queue<ScoreIdx, std::vector<ScoreIdx>, std::greater<ScoreIdx>> local_heap;

            #pragma omp for nowait
            for (uint64_t i = 0; i < tile_records_count; ++i) {
                float rec_theta = (static_cast<float>(records[i].quantized_theta) / 65535.0f) * (2.0f * M_PI);
                // Angular similarity approximation: cos(theta_q - theta_i)
                float score = std::cos(query_theta - rec_theta);

                if (local_heap.size() < static_cast<size_t>(k)) {
                    local_heap.push({score, i});
                } else if (score > local_heap.top().first) {
                    local_heap.pop();
                    local_heap.push({score, i});
                }
            }

            #pragma omp critical
            {
                while (!local_heap.empty()) {
                    auto item = local_heap.top();
                    local_heap.pop();
                    if (topk_heap.size() < static_cast<size_t>(k)) {
                        topk_heap.push(item);
                    } else if (item.first > topk_heap.top().first) {
                        topk_heap.pop();
                        topk_heap.push(item);
                    }
                }
            }
        }

        int idx = topk_heap.size() - 1;
        while (!topk_heap.empty() && idx >= 0) {
            out_scores[idx] = topk_heap.top().first;
            out_indices[idx] = topk_heap.top().second;
            topk_heap.pop();
            idx--;
        }
    }

    void torky_destroy_streamer(void* streamer_ptr) {
        if (streamer_ptr) {
            delete reinterpret_cast<TorkyTileStreamer*>(streamer_ptr);
        }
    }
}
