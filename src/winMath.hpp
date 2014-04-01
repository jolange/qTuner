#ifndef WINMATH_HPP_
#define WINMATH_HPP_
// lack of Windows math functions
#ifdef _WIN32

namespace qTuner{
double log2(double d);
double round(double d);
}
#endif // _WIN32
#endif // WINMATH_HPP_
