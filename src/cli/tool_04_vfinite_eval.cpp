#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(4, "torky-vfinite-eval", "Non-Singular Finite-Core Operator V_finite(r) Evaluator");
    double r = (argc > 1) ? std::stod(argv[1]) : 0.0;
    double pot = torky::continuum::v_finite_ev(r);
    std::cout << "Coordinate r: " << r << " nm\n";
    std::cout << "V_finite(" << r << "): " << pot << " eV\n";
    std::cout << "Capped at r=0: " << torky::continuum::V_FINITE_ZERO_EV << " eV (< inf, zero crash)\n";
    return (pot > 0.0 && pot <= 11.385) ? 0 : 1;
}
