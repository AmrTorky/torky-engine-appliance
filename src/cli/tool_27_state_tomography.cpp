#include "torky_cli/cli_common.hpp"

int main() {
    torky::cli::print_banner(27, "torky-state-tomography", "Quantum State Density Matrix Purity & Trace Verifier");
    // 2x2 pure ground density matrix [[1, 0], [0, 0]]
    double rho[2][2] = {{1.0, 0.0}, {0.0, 0.0}};
    double trace = rho[0][0] + rho[1][1];
    double purity = (rho[0][0]*rho[0][0] + rho[0][1]*rho[1][0] + rho[1][0]*rho[0][1] + rho[1][1]*rho[1][1]);
    std::cout << "Density Matrix Trace: " << trace << " (Strictly == 1.0)\n";
    std::cout << "State Purity Tr(rho^2): " << purity << " (Pure Quantum State)\n";
    return (trace == 1.0 && purity == 1.0) ? 0 : 1;
}
