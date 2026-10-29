#include "torky_cli/cli_common.hpp"

int main() {
    torky::cli::print_banner(23, "torky-phase-coherence", "16-Bit Compact Phase Register Quantizer & Decoder");
    constexpr double phase_rad = 0.00487;
    uint16_t quantized_phase = static_cast<uint16_t>(phase_rad * 1e6);
    double decoded_phase = quantized_phase / 1e6;
    std::cout << "Input Radians: " << phase_rad << " rad\n";
    std::cout << "16-Bit Register Encoding: 0x" << std::hex << quantized_phase << std::dec << " (" << quantized_phase << ")\n";
    std::cout << "Decoded Value: " << decoded_phase << " rad | Quantization Loss: " << std::abs(phase_rad - decoded_phase) << " rad\n";
    return 0;
}
