CXX ?= g++
CXXFLAGS = -O3 -std=c++20 -march=native -Wall -Wextra -Iinclude -pthread
TARGET = build/torky-appliance-cli

CLI_SRCS = $(wildcard src/cli/*.cpp)
CLI_BINS = $(patsubst src/cli/%.cpp,build/%,$(CLI_SRCS))

all: $(TARGET) cli-tools

$(TARGET): src/torky_cli_kernel.cpp include/torky_continuum_core.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) src/torky_cli_kernel.cpp -o $(TARGET)

cli-tools: $(CLI_BINS)

build/%: src/cli/%.cpp include/torky_continuum_core.hpp include/torky_cli/cli_common.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -Iinclude -Iinclude/torky_cli $< -o $@

test: all
	./$(TARGET)
	./build/tool_01_mmap_alloc 500000
	./build/tool_02_cell_pack
	./build/tool_03_bigint_jump 100
	./build/tool_04_vfinite_eval 0.0

clean:
	rm -rf build local_verification_certificate.json
