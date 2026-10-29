#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(6, "torky-hessian-clamp", "Autopoietic Hessian Condition Number Bound & Curvature Clamp");
    double l_max = (argc > 1) ? std::stod(argv[1]) : 250.0;
    double l_min = (argc > 2) ? std::stod(argv[2]) : 5.0;
    double kappa = torky::continuum::clamp_hessian_condition(l_max, l_min);
    std::cout << "Raw Eigenvalues: lambda_max=" << l_max << ", lambda_min=" << l_min << "\n";
    std::cout << "Clamped Condition Number kappa(H): " << kappa << "\n";
    std::cout << "Hard Bound Ceiling: <= " << torky::continuum::HESSIAN_KAPPA_MAX << "\n";
    return (kappa <= torky::continuum::HESSIAN_KAPPA_MAX) ? 0 : 1;
}
