#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(13, "torky-kretschmann-cap", "Black Hole Non-Singular Kretschmann Scalar Curvature Bound");
    double r = (argc > 1) ? std::stod(argv[1]) : 0.0;
    double r_eff = std::sqrt(r * r + torky::continuum::EPSILON_NM2);
    double k_scalar = 48.0 / std::pow(r_eff, 6);
    std::cout << "Radius r: " << r << " nm (r_eff: " << r_eff << " nm)\n";
    std::cout << "Kretschmann Invariant K(r): " << k_scalar << " nm^-4\n";
    std::cout << "Curvature at Horizon / Center: Strictly Finite (< inf)\n";
    return 0;
}
