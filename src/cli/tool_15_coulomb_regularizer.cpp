#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(15, "torky-coulomb-regularizer", "Non-Singular Coulomb Energy Field Operator");
    double q1 = (argc > 1) ? std::stod(argv[1]) : 1.0;
    double q2 = (argc > 2) ? std::stod(argv[2]) : 1.0;
    double r  = (argc > 3) ? std::stod(argv[3]) : 0.0;
    double energy = (q1 * q2) / std::sqrt(r * r + torky::continuum::EPSILON_NM2);
    std::cout << "Charges: q1=" << q1 << ", q2=" << q2 << " at separation r=" << r << " nm\n";
    std::cout << "Regularized Coulomb Potential: " << energy << " a.u.\n";
    std::cout << "Peak Force / Energy at r=0: " << (q1 * q2) / std::sqrt(torky::continuum::EPSILON_NM2) << " a.u. (< inf)\n";
    return 0;
}
