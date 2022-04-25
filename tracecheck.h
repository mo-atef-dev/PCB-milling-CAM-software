#ifndef TRACECHECK_H_INCLUDED
#define TRACECHECK_H_INCLUDED

#include "tracebmp.h"
#include <cstdlib>

/// Macro that returns 1 if x2 > x1, 0 if x2 == x1, and -1 if x2 < x1
#define GET_DIR(x2, x1) (2*((signed int)x2 > (signed int)x1) - 1 + ((signed int)x2 == (signed int)x1))

struct CheckResult
{
    int code;
    unsigned int nGoTo;
    unsigned int nLineTo;
    unsigned int nTotal;
    unsigned int nErrors;
    bitmap_image img;
};

int BriefCheck(vector<Command>, bitmap_image);
CheckResult DetailedCheck(vector<Command>, bitmap_image);


#endif // TRACECHECK_H_INCLUDED
