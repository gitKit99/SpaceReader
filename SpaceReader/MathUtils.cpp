#include "MathUtils.h"

double MathUtils::map(double var, double begin, double end,
    double targetBegin, double targetEnd)
{
    return (targetBegin + ((var - begin) * (targetEnd - targetBegin))
        / (end - begin));
}