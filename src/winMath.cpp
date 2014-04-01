#include "winMath.hpp"
#include <cmath>
namespace qTuner{
double log2(double d)
{
   return log(d)/log(2.);
}
double round(double d)
{
   return floor(d + 0.5);
}
}

