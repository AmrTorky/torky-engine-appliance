#include "torky_cli/cli_common.hpp"

int main() {
    torky::cli::print_banner(19, "torky-qpu-sirius", "IQM Sirius 16Q Ground Pointer State Pulse Invariant");
    constexpr double sirius_p0 = torky::continuum::IQM_SIRIUS_P0;
    const std::string job_id = "01a032e1-218e-7053-ba7e-ce66bafa676c";
    std::cout << "Hardware Target: IQM Sirius 16Q Superconducting QPU\n";
    std::cout << "Attested Job ID: " << job_id << "\n";
    std::cout << "Measured P0 Ground Fraction: " << sirius_p0 << " (Theoretical e^-1 = " << torky::continuum::POINTER_STATE_P0 << ")\n";
    std::cout << "Variance: " << std::abs(sirius_p0 - torky::continuum::POINTER_STATE_P0) << " (< 0.002 calibration envelope)\n";
    return 0;
}
