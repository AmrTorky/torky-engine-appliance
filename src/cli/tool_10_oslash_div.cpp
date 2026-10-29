#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(10, "torky-oslash-div", "Non-Singular oslash_epsilon Regularized Division Operator");
    double num = (argc > 1) ? std::stod(argv[1]) : 1.0;
    double r   = (argc > 2) ? std::stod(argv[2]) : 0.0;
    double res = torky::continuum::oslash_epsilon(num, r);
    std::cout << "Operation: " << num << " /_eps sqrt(" << r << "^2 + " << torky::continuum::EPSILON_NM2 << ")\n";
    std::cout << "Result: " << res << " (< inf at r=0, classical 1/0 replaced)\n";
    return 0;
}
