#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(7, "torky-cardano-viete", "Analytical Depressed Cubic Cardano-Viète Root Solver");
    double p = (argc > 1) ? std::stod(argv[1]) : -3.5;
    double q = (argc > 2) ? std::stod(argv[2]) : 1.2;
    auto t0 = std::chrono::high_resolution_clock::now();
    auto res = torky::continuum::solve_cardano_viete(p, q);
    auto t1 = std::chrono::high_resolution_clock::now();
    double us = std::chrono::duration<double, std::micro>(t1 - t0).count();
    std::cout << "Polynomial: z^3 + (" << p << ")z + (" << q << ") = 0\n";
    std::cout << "Roots: [" << res.roots[0] << ", " << res.roots[1] << ", " << res.roots[2] << "]\n";
    std::cout << "Residual: " << res.residual_kcal_mol << " kcal/mol | Latency: " << us << " us\n";
    return 0;
}
