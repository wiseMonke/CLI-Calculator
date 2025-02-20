#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <math.h>

// Constants
static const double PI = 3.14159265358979323846;
static const double TWO_PI = 2.0 * PI;
static const double HALF_PI = 0.5 * PI;
static const double LN2 = 0.6931471805599453; // ln(2)

// Reduce x to [-π/4, π/4] and return quadrant
static inline double reduceAngle(double x, int *quadrant)
{
    x = fmod(x, TWO_PI); // Reduce to [0, 2π]
    if (x < 0)
        x += TWO_PI; // Ensure x is in [0, 2π]

    *quadrant = (int)(x / HALF_PI) % 4; // Determine quadrant
    double reducedX = fmod(x, PI);      // Reduce to [0, π]

    if (*quadrant == 1 || *quadrant == 3)
        reducedX = PI - reducedX; // Reflect in quadrant 1 & 3

    if (*quadrant >= 2)
        reducedX = -reducedX; // Negate in quadrant 2 & 3

    return reducedX;
}

// Sine approximation using Remez polynomial
static inline double customSIN(double x)
{
    int quadrant;
    x = reduceAngle(x, &quadrant);

    const double c1 = -1.66666666666666324348e-01;
    const double c2 = 8.33333333332248946124e-03;
    const double c3 = -1.98412698298579493134e-04;
    const double c4 = 2.75573137070700676789e-06;
    const double c5 = -2.50507602534068634195e-08;
    const double c6 = 1.58969099521155010221e-10;

    double x2 = x * x;
    double result = x * (1.0 + x2 * (c1 + x2 * (c2 + x2 * (c3 + x2 * (c4 + x2 * (c5 + x2 * c6))))));

    return result;
}

// Cosine approximation using Remez polynomial
static inline double customCOS(double x)
{
    int quadrant;
    x = reduceAngle(x, &quadrant);

    const double c1 = -0.5;
    const double c2 = 4.16666666666666019037e-02;
    const double c3 = -1.38888888888741095749e-03;
    const double c4 = 2.48015872894767294178e-05;
    const double c5 = -2.75573143513906633035e-07;
    const double c6 = 2.08757232129817482790e-09;

    double x2 = x * x;
    double result = 1.0 + x2 * (c1 + x2 * (c2 + x2 * (c3 + x2 * (c4 + x2 * (c5 + x2 * c6)))));

    return (quadrant == 1 || quadrant == 2) ? -result : result;
}

// Tangent approximation
static inline double customTAN(double x)
{
    double cos_val = customCOS(x);
    if (fabs(cos_val) < 1e-10)
        return INFINITY; // Avoid division by zero
    return customSIN(x) / cos_val;
}

// Exponential function approximation
static inline double customEXP(double x)
{
    if (x > 709.78)
        return INFINITY;
    if (x < -745.13)
        return 0.0;

    // Range reduction: x = n * ln(2) + r
    const double ln2 = 0.6931471805599453;
    int n = round(x / ln2);
    double r = x - n * ln2;

    // Polynomial approximation for exp(r) on [-0.5, 0.5]
    const double c1 = 1.0;
    const double c2 = 1.0;
    const double c3 = 0.5;
    const double c4 = 0.16666666666666666;
    const double c5 = 0.041666666666666664;
    const double c6 = 0.008333333333333333;
    const double c7 = 0.001388888888888889;
    const double c8 = 0.0001984126984126984;
    const double c9 = 0.0000248015873015873;
    const double c10 = 0.00000275573192239859;

    double exp_r = c1 + r * (c2 + r * (c3 + r * (c4 + r * (c5 + r * (c6 + r * (c7 + r * (c8 + r * (c9 + r * c10))))))));

    return ldexp(exp_r, n);
}

// Natural logarithm approximation using minimax polynomial on [0.5, 1]
static inline double customLN(double x)
{
    if (x <= 0.0)
        return NAN; // ln(x) is undefined for x <= 0

    // Range reduction: x = m * 2^exp, where m ∈ [0.5, 1)
    int exp;
    double m = frexp(x, &exp); // Normalize x to [0.5, 1)
    m *= 2.0;                  // Scale m to [1, 2)
    exp--;                     // Adjust exponent accordingly

    // Transform m to z ∈ [-1/3, 1/3] for better polynomial approximation
    double z = (m - 1) / (m + 1);
    double z2 = z * z;

    // Minimax polynomial coefficients for ln((1 + z) / (1 - z))
    const double c1 = 2.0;
    const double c2 = 0.6666666666666735;
    const double c3 = 0.3999999999940942;
    const double c4 = 0.2857142874366239;
    const double c5 = 0.2222219843214979;
    const double c6 = 0.1818357216161805;
    const double c7 = 0.1531383769920937;
    const double c8 = 0.1479819860511659;

    double lnM = z * (c1 + z2 * (c2 + z2 * (c3 + z2 * (c4 + z2 * (c5 + z2 * (c6 + z2 * (c7 + z2 * c8)))))));

    // Reconstruct ln(x) = ln(m) + exp * ln(2)
    return lnM + exp * LN2;
}

// Hyperbolic sine approximation
static inline double customSINH(double x)
{
    double ex = customEXP(x);
    double e_minus_x = customEXP(-x);
    return (ex - e_minus_x) / 2.0;
}

// Hyperbolic cosine approximation
static inline double customCOSH(double x)
{
    double ex = customEXP(x);
    double e_minus_x = customEXP(-x);
    return (ex + e_minus_x) / 2.0;
}

// Hyperbolic tangent approximation
static inline double customTANH(double x)
{
    double ex = customEXP(x);
    double e_minus_x = customEXP(-x);
    return (ex - e_minus_x) / (ex + e_minus_x);
}

// Inverse sine approximation using minimax polynomial on [-1, 1]
static inline double customASIN(double x)
{
    if (x < -1.0 || x > 1.0)
        return NAN; // asin(x) is undefined for |x| > 1

    // Use symmetry: asin(-x) = -asin(x)
    if (x < 0)
        return -customASIN(-x);

    // Polynomial coefficients for asin(x) on [0, 1]
    const double c1 = 1.0;
    const double c2 = 0.16666666666666666;
    const double c3 = 0.075;
    const double c4 = 0.04464285714285714;
    const double c5 = 0.03038194444444444;
    const double c6 = 0.02237215909090909;
    const double c7 = 0.01735276442307692;

    double x2 = x * x;
    return x * (c1 + x2 * (c2 + x2 * (c3 + x2 * (c4 + x2 * (c5 + x2 * (c6 + x2 * c7))))));
}

// Inverse cosine approximation
static inline double customACOS(double x)
{
    return HALF_PI - customASIN(x); // acos(x) = π/2 - asin(x)
}

// Inverse tangent approximation using minimax polynomial on [-1, 1]
static inline double customATAN(double x)
{
    // Use symmetry: atan(-x) = -atan(x)
    if (x < 0)
        return -customATAN(-x);

    if (x > 1.0)
        return HALF_PI - customATAN(1.0 / x); // atan(x) = π/2 - atan(1/x) for x > 1

    // Polynomial coefficients for atan(x) on [0, 1]
    const double c1 = 1.0;
    const double c2 = -0.3333333333333333;
    const double c3 = 0.2;
    const double c4 = -0.14285714285714285;
    const double c5 = 0.1111111111111111;
    const double c6 = -0.09090909090909091;
    const double c7 = 0.07692307692307693;

    double x2 = x * x;
    return x * (c1 + x2 * (c2 + x2 * (c3 + x2 * (c4 + x2 * (c5 + x2 * (c6 + x2 * c7))))));
}

// Inverse hyperbolic sine approximation
static inline double customASINH(double x)
{
    return customLN(x + sqrt(x * x + 1)); // asinh(x) = ln(x + sqrt(x^2 + 1))
}

// Inverse hyperbolic cosine approximation
static inline double customACOSH(double x)
{
    if (x < 1.0)
        return NAN;                       // acosh(x) is undefined for x < 1
    return customLN(x + sqrt(x * x - 1)); // acosh(x) = ln(x + sqrt(x^2 - 1))
}

// Inverse hyperbolic tangent approximation
static inline double customATANH(double x)
{
    if (x <= -1.0 || x >= 1.0)
        return NAN;                           // atanh(x) is undefined for |x| >= 1
    return 0.5 * customLN((1 + x) / (1 - x)); // atanh(x) = 0.5 * ln((1 + x) / (1 - x))
}

// Power function approximation
static inline double customPOW(double a, double b)
{
    if (a == 0.0 && b > 0)
        return 0.0; // 0^b = 0 for b > 0
    if (a < 0.0 && b != (int)b)
        return NAN; // a^b is undefined for a < 0 and non-integer b

    return customEXP(b * customLN(a)); // a^b = e^(b * ln(a))
}

// Logarithm with base approximation
static inline double customLogBase(double a, double b)
{
    if (a <= 0.0 || b <= 0.0 || b == 1.0)
        return NAN; // log_a(b) is undefined for a <= 0, b <= 0, or b == 1

    return customLN(b) / customLN(a); // log_a(b) = ln(a) / ln(b)
}

#endif