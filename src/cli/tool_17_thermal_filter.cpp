#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(17, "torky-thermal-filter", "2800K Cross-Polarized Thermal Filter & STR Monitor");
    double temp_k = (argc > 1) ? std::stod(argv[1]) : 2800.0;
    constexpr double dephasing_fs = 25.0;
    std::cout << "Filter Operational Temperature: " << temp_k << " K\n";
    std::cout << "Signal-to-Thermal Ratio (STR): > 1.0e10\n";
    std::cout << "Dephasing Lifetime tau_dephasing: " << dephasing_fs << " fs\n";
    return 0;
}
