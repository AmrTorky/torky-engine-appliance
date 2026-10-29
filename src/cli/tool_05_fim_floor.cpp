#include "torky_cli/cli_common.hpp"

int main(int argc, char* argv[]) {
    torky::cli::print_banner(5, "torky-fim-floor", "Fisher Information Metric (FIM) Floor & Non-Vanishing Gradient Monitor");
    double r = (argc > 1) ? std::stod(argv[1]) : 0.0;
    double fim = torky::continuum::regularized_fim(r);
    std::cout << "Evaluated Radius: " << r << " nm\n";
    std::cout << "FIM Eigenvalue: " << fim << " nm^-2\n";
    std::cout << "Guaranteed Floor: " << torky::continuum::FIM_LAMBDA_MIN << " nm^-2 (e^2 > 0)\n";
    return (fim >= torky::continuum::FIM_LAMBDA_MIN) ? 0 : 1;
}
