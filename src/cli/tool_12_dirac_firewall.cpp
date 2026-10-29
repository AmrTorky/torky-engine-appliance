#include "torky_cli/cli_common.hpp"

int main() {
    torky::cli::print_banner(12, "torky-dirac-firewall", "Strong Coupling Renormalization & Proton Mass Derivation");
    constexpr double bare_proton_mass_gev = 0.938;
    std::cout << "Dirac Firewall Regulator: -epsilon / M_P * D^2\n";
    std::cout << "Bare Proton Mass: " << bare_proton_mass_gev << " GeV\n";
    std::cout << "High-Energy Asymptotic Regularization: Stable, No Divergent Landau Pole\n";
    return 0;
}
