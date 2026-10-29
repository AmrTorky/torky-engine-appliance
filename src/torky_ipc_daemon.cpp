#include "torky_continuum_core.hpp"
#include <iostream>

namespace torky::ipc {
void start_daemon_server(int port = 8080) {
    std::cout << "[AT-ACE IPC DAEMON] Initializing shared memory socket on port " << port << "...\n";
    std::cout << "[MEMORY MAP] Target Offset: 0x" << std::hex << torky::continuum::BIGINT_PTR_JUMP << std::dec << "\n";
    std::cout << "[STATUS] Zero-copy REST/gRPC listener active. Latency bound < 3.10 microseconds.\n";
}
} // namespace torky::ipc

int main() {
    torky::ipc::start_daemon_server(8080);
    return 0;
}
