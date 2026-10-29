#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(31, "torky-flags-bitmask", "TORKY_CELL_DTYPE 1-Byte Hardware Bitmask Mutator & Inspector");
    uint8_t flags = (argc > 1) ? static_cast<uint8_t>(std::stoul(argv[1], nullptr, 0)) : 0b00000111;
    bool clamped = (flags & 0b00000001) != 0;
    bool no_nan  = (flags & 0b00000010) != 0;
    bool worm    = (flags & 0b00000100) != 0;
    std::cout << "Raw Flags Byte: 0x" << std::hex << (int)flags << std::dec << "\n";
    std::cout << "Bit 0 (Autopoietic Clamped): " << (clamped ? "ACTIVE (1)" : "DISABLED (0)") << "\n";
    std::cout << "Bit 1 (Zero-NaN Protected):  " << (no_nan ? "ACTIVE (1)" : "DISABLED (0)") << "\n";
    std::cout << "Bit 2 (WORM Immutable Seal): " << (worm ? "ACTIVE (1)" : "DISABLED (0)") << "\n";
    return (clamped && no_nan && worm) ? 0 : 1;
}
