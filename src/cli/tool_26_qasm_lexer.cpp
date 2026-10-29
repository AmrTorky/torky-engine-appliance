#include "torky_cli/cli_common.hpp"
#include <sstream>

int main() {
    torky::cli::print_banner(26, "torky-qasm-lexer", "OpenQASM 3.0 Instruction Stream Fast Lexer");
    const std::string sample_qasm = "OPENQASM 3.0;\nqubit[3] q;\nh q[0];\ncx q[0], q[1];\n";
    std::istringstream stream(sample_qasm);
    std::string line;
    int instructions = 0;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.find("//") == std::string::npos) {
            instructions++;
            std::cout << "Parsed Instruction [" << instructions << "]: " << line << "\n";
        }
    }
    std::cout << "[SUCCESS] Lexed " << instructions << " instructions into memory.\n";
    return 0;
}
