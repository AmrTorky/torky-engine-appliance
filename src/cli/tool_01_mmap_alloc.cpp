#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(1, "torky-mmap-alloc", "POSIX Anonymous & File-Backed Zero-Copy mmap Allocator");
    size_t num_cells = (argc > 1) ? std::stoull(argv[1]) : 1000000;
    auto t0 = std::chrono::high_resolution_clock::now();
    torky::continuum::PosixContinuumArena arena("/tmp/torky_mmap_test.bin", num_cells);
    auto t1 = std::chrono::high_resolution_clock::now();
    double us = std::chrono::duration<double, std::micro>(t1 - t0).count();
    std::cout << "[SUCCESS] Allocated " << num_cells << " cells ("
              << (num_cells * sizeof(torky::continuum::TORKY_CELL_DTYPE)) / (1024.0 * 1024.0)
              << " MiB) in " << us << " us.\n";
    return arena.is_valid() ? 0 : 1;
}
