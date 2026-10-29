#ifndef TORKY_CLI_COMMON_HPP
#define TORKY_CLI_COMMON_HPP

#include "torky_continuum_core.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

namespace torky::cli {

inline void print_banner(int tool_id, const std::string& name, const std::string& desc) {
    std::cout << "==============================================================================\n";
    std::cout << "[TORKY APPLIANCE CLI #" << std::setw(2) << std::setfill('0') << tool_id 
              << "] " << name << "\n";
    std::cout << "Role: " << desc << "\n";
    std::cout << "Governance: ISO/IEC 17025 (" << "1dd69a153b00" << ") | Profile C Sealed\n";
    std::cout << "==============================================================================\n";
}

} // namespace torky::cli
#endif // TORKY_CLI_COMMON_HPP
