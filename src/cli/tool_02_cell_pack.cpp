#include "torky_cli/cli_common.hpp"

int main() {
    torky::cli::print_banner(2, "torky-cell-pack", "9-Byte TORKY_CELL_DTYPE Packing & Unpacking Validator");
    torky::continuum::TORKY_CELL_DTYPE cell{100, 738, 487, 0b00000111};
    std::cout << "Cell Struct Size: " << sizeof(cell) << " bytes (Strict 9B requirement)\n";
    std::cout << "Manifold Coord: " << cell.quantized_manifold_coord
              << " | FIM Mantissa: " << cell.fim_metric_mantissa
              << " | Phase Coherence: " << cell.phase_coherence_rad
              << " | Flags: 0x" << std::hex << (int)cell.autopoietic_status_flags << std::dec << "\n";
    return (sizeof(cell) == 9) ? 0 : 1;
}
