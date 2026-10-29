#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(25, "torky-transmon-sim", "Non-Linear Josephson Junction Eigenfrequency Estimator");
    double ej = (argc > 1) ? std::stod(argv[1]) : 15.0; // GHz
    double ec = (argc > 2) ? std::stod(argv[2]) : 0.25; // GHz
    double omega_01 = std::sqrt(8.0 * ej * ec) - ec;
    std::cout << "Josephson Energy Ej: " << ej << " GHz | Charging Energy Ec: " << ec << " GHz\n";
    std::cout << "Fundamental Transition Frequency omega_01: " << omega_01 << " GHz\n";
    std::cout << "Anharmonicity: " << -ec << " GHz (Unperturbed by singularity)\n";
    return 0;
}
