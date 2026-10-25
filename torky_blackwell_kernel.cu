/*
 * TORKY AUTOPOIETIC CONTINUUM ENGINE - NVIDIA CUDA WARP KERNEL
 * Target: Blackwell B200/B300 / Hopper H100 / H200
 * Bypasses FP64 hardware throttling via analytical FP32 Cardano-Viete evaluation
 */
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

extern "C" __global__ void torky_warp_reconstruct_kernel(
    const uint8_t* __restrict__ d_packed_bytes,
    float* __restrict__ d_out_vectors,
    const unsigned long long total_records)
{
    unsigned long long idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= total_records) return;

    unsigned long long byte_offset = idx * 9ULL;
    uint8_t mask = d_packed_bytes[byte_offset];
    uint16_t r_raw = *reinterpret_cast<const uint16_t*>(&d_packed_bytes[byte_offset + 1]);
    uint16_t th_raw = *reinterpret_cast<const uint16_t*>(&d_packed_bytes[byte_offset + 3]);

    float r = __fdividef((float)r_raw, 65535.0f);
    float theta = (float)th_raw * (6.283185307f / 65535.0f);

    // Cardano-Viete exact trigonometric reconstruction
    d_out_vectors[idx] = r * 1.154700538f * __cosf(theta * 0.333333333f);
}
