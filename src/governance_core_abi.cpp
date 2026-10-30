#include <cstdint>
#include <cmath>
#include <cstring>

#if defined(__GNUC__) || defined(__clang__)
  #define TORKY_EXPORT __attribute__((visibility("default")))
#else
  #define TORKY_EXPORT
#endif

namespace {
    constexpr double   SECRET_LA      = 0.36787944117144233;
    constexpr double   SECRET_EPSILON = 0.016;
    constexpr double   SECRET_V0      = 1.440;
    constexpr double   SECRET_KAPPA   = 18.50;
    constexpr uint32_t GOLDEN_PRIME   = 0x9E3779B9U;
    constexpr uint32_t GOLDEN_INV     = 0x144CBC89U;
}

extern "C" {

TORKY_EXPORT int gov_init_stream(uint64_t seed) {
    return (seed != 0) ? 0 : -1;
}

TORKY_EXPORT uint8_t gov_pack_ceos_byte(uint8_t gate_0_to_3, uint8_t gen_0_to_63) {
    return static_cast<uint8_t>(((gate_0_to_3 & 0x03U) << 6U) | (gen_0_to_63 & 0x3FU));
}

TORKY_EXPORT uint32_t gov_golden_syndrome(uint64_t idx64) {
    return static_cast<uint32_t>(idx64 & 0xFFFFFFFFULL) * GOLDEN_PRIME;
}

TORKY_EXPORT uint32_t gov_recover_low32(uint32_t syndrome) {
    return syndrome * GOLDEN_INV;
}

TORKY_EXPORT double gov_eval_potential(double r_nm) {
    return SECRET_V0 / std::sqrt((r_nm * r_nm) + SECRET_EPSILON);
}

TORKY_EXPORT double gov_reconstruct_probe(double h_norm) {
    const double h_clamped = (h_norm > 1.0) ? 1.0 : ((h_norm < -1.0) ? -1.0 : h_norm);
    const double sgn = (h_clamped >= 0.0) ? 1.0 : -1.0;
    const double arg = - (3.0 * std::sqrt(3.0) * SECRET_EPSILON * h_clamped) / 8.0;
    return sgn * (2.0 / std::sqrt(3.0)) * std::cos(std::acos(arg) / 3.0);
}

TORKY_EXPORT double gov_audit_hessian(double lmax, double lmin) {
    const double fim_floor = 1.0 / (SECRET_LA * SECRET_LA);
    const double safe_min = (lmin < fim_floor) ? fim_floor : lmin;
    const double raw_k = lmax / safe_min;
    if (raw_k <= SECRET_KAPPA) return raw_k;
    const double alpha = (lmax - (SECRET_KAPPA * safe_min)) / (SECRET_KAPPA - 1.0);
    return (lmax + alpha) / (safe_min + alpha);
}

} // extern "C"
