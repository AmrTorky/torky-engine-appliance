#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(22, "torky-crosstalk-bound", "Multi-Qubit Phase Noise & Crosstalk Bound Invariant");
    double phase_drift = (argc > 1) ? std::stod(argv[1]) : 0.00487;
    constexpr double sigma_phase = 0.00487;
    constexpr double max_drift_ceiling = 0.01050;
    std::cout << "Evaluated Inter-Qubit Phase Drift: " << phase_drift << " rad\n";
    std::cout << "Nominal Noise Floor sigma_phase: " << sigma_phase << " rad\n";
    std::cout << "Allowed Drift Upper Limit: <= " << max_drift_ceiling << " rad\n";
    return (phase_drift <= max_drift_ceiling) ? 0 : 1;
}
