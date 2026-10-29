#include <cstdint>
#include <cmath>
#include <cstring>

#if defined(__GNUC__) || defined(__clang__)
  #define TORKY_EXPORT __attribute__((visibility("default")))
#else
  #define TORKY_EXPORT
#endif

namespace {
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

TORKY_EXPORT uint32_t gov_golden_syndrome(uint64_t idx64) {
    return static_cast<uint32_t>(idx64 & 0xFFFFFFFFULL) * GOLDEN_PRIME;
}

TORKY_EXPORT uint32_t gov_recover_low32(uint32_t syndrome) {
    return syndrome * GOLDEN_INV;
}

TORKY_EXPORT double gov_eval_potential(double r_nm) {
    return SECRET_V0 / std::sqrt((r_nm * r_nm) + SECRET_EPSILON);
}

TORKY_EXPORT double gov_audit_hessian(double lmax, double lmin) {
    const double safe_min = (lmin < 7.38905609893065) ? 7.38905609893065 : lmin;
    const double raw_k = lmax / safe_min;
    if (raw_k <= SECRET_KAPPA) return raw_k;
    const double alpha = (lmax - (SECRET_KAPPA * safe_min)) / (SECRET_KAPPA - 1.0);
    return (lmax + alpha) / (safe_min + alpha);
}

} // extern "C"
