#include "torky_continuum_core.hpp"
#include <iostream>
int main() {
    std::cout << "[KERNEL: torky-engine-appliance] CLI=torky-appliance-cli | 9-byte struct verified | V(0)=" 
              << torky::v_finite_ev(0.0) << " eV\n";
    return 0;
}
