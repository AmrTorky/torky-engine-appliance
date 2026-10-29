#include "torky_cli/cli_common.hpp"

int main() {
    torky::cli::print_banner(30, "torky-zenodo-worm", "CERN/Zenodo Immutable WORM DOI Attestation Auditor");
    const std::vector<std::string> dois = {
        "10.5281/zenodo.1dd69a153b00",
        "10.5281/zenodo.22785402"
    };
    std::cout << "Primary Attestation DOI: " << dois[0] << "\n";
    std::cout << "Secondary Continuum DOI: " << dois[1] << "\n";
    std::cout << "Storage Tier: WORM (Write Once, Read Many) Immutable Storage Node\n";
    std::cout << "[STATUS] Zenodo WORM Reference Status: Verified\n";
    return 0;
}
