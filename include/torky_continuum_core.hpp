#ifndef TORKY_CONTINUUM_CORE_HPP
#define TORKY_CONTINUUM_CORE_HPP

#include <cmath>
#include <cstdint>
#include <cstring>
#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace torky::continuum {

inline constexpr double L_A_NM             = 0.36787944117144233;
inline constexpr double EPSILON_NM2        = 0.016;
inline constexpr double XI_T_NORM          = 1.02e60;
inline constexpr double V_FINITE_ZERO_EV   = 11.384;
inline constexpr double FIM_LAMBDA_MIN     = 7.38905609893065;
inline constexpr double HESSIAN_KAPPA_MAX  = 18.50;
inline constexpr double POINTER_STATE_P0   = 0.36787944117144233;
inline constexpr uint64_t BIGINT_PTR_JUMP  = 0x14f46b04000ULL;

#pragma pack(push, 1)
struct TORKY_CELL_DTYPE {
    uint32_t quantized_manifold_coord; 
    uint16_t fim_metric_mantissa;      
    uint16_t phase_coherence_rad;      
    uint8_t  autopoietic_status_flags; 
};
#pragma pack(pop)

static_assert(sizeof(TORKY_CELL_DTYPE) == 9, "TORKY_CELL_DTYPE must be strictly 9 bytes.");

[[nodiscard]] inline constexpr double v_finite_ev(double r_nm) noexcept {
    constexpr double k_scale = V_FINITE_ZERO_EV * std::sqrt(EPSILON_NM2);
    return k_scale / std::sqrt((r_nm * r_nm) + EPSILON_NM2);
}

struct CubicRoots {
    std::array<double, 3> roots;
    double residual_kcal_mol;
};

[[nodiscard]] inline CubicRoots solve_cardano_viete(double p, double q) noexcept {
    CubicRoots result{{0.0, 0.0, 0.0}, 0.0};
    constexpr double PI = 3.14159265358979323846;
    const double p_reg = (p >= -1e-12) ? -std::sqrt((p * p) + EPSILON_NM2) : p;
    const double m = 2.0 * std::sqrt(-p_reg / 3.0);
    double arg = (3.0 * q) / (2.0 * p_reg) * std::sqrt(-3.0 / p_reg);
    if (arg > 1.0) arg = 1.0;
    if (arg < -1.0) arg = -1.0;
    const double theta = std::acos(arg) / 3.0;
    for (int k = 0; k < 3; ++k) {
        result.roots[k] = m * std::cos(theta - (2.0 * PI * k) / 3.0);
    }
    const double z0 = result.roots[0];
    result.residual_kcal_mol = std::abs((z0 * z0 * z0) + (p_reg * z0) + q) * 1e-4;
    return result;
}

} // namespace torky::continuum

#endif // TORKY_CONTINUUM_CORE_HPP
