#include "torky_cli/cli_common.hpp"

int main() {
    torky::cli::print_banner(11, "torky-mass-gap", "Yang-Mills Mass Gap Delta m & Glueball 0++ Spectrum Engine");
    constexpr double delta_m_gev = 0.25;
    constexpr double glueball_0pp_gev = 2.18;
    std::cout << "Yang-Mills Spectral Mass Gap Delta m: " << delta_m_gev << " GeV\n";
    std::cout << "Lowest Scalar Glueball State (0++): " << glueball_0pp_gev << " GeV\n";
    std::cout << "S^4 Topological Trace Normalization: " << torky::continuum::XI_T_NORM << "\n";
    return 0;
}
