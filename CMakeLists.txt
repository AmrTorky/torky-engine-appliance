cmake_minimum_required(VERSION 3.16)
project(torky-engine-appliance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(torky_kernel_bin src/torky_cli_kernel.cpp)
target_compile_options(torky_kernel_bin PRIVATE -O3 -Wall -Wextra -pthread -march=native)

add_executable(torky_ipc_daemon_bin src/torky_ipc_daemon.cpp)
target_compile_options(torky_ipc_daemon_bin PRIVATE -O3 -Wall -Wextra -pthread -march=native)

add_executable(torky_lakehouse_bin src/torky_lakehouse_bridge.cpp)
target_compile_options(torky_lakehouse_bin PRIVATE -O3 -Wall -Wextra -pthread -march=native)

add_executable(torky_hardware_sim_bin src/torky_hardware_simulator.cpp)
target_compile_options(torky_hardware_sim_bin PRIVATE -O3 -Wall -Wextra -pthread -march=native)

add_executable(torky_security_bin src/torky_ontology_security.cpp)
target_compile_options(torky_security_bin PRIVATE -O3 -Wall -Wextra -pthread -march=native)
