#include "torky_cli/cli_common.hpp"

int main() {
    torky::cli::print_banner(8, "torky-triple-scale", "Master Scale Triple Invariant Validation Engine");
    std::cout << "Invariant l_a (e^-1 nm): " << std::setprecision(12) << torky::continuum::L_A_NM << " nm\n";
    std::cout << "Invariant epsilon: " << torky::continuum::EPSILON_NM2 << " nm^2\n";
    std::cout << "Invariant Xi_T,norm (Topological Trace): " << torky::continuum::XI_T_NORM << "\n";
    std::cout << "Ground Pointer State P0: " << torky::continuum::POINTER_STATE_P0 << "\n";
    return 0;
}
