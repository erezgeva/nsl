/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2018 Erez Geva */

#ifndef __NSL_STATISTICS__H__
#define __NSL_STATISTICS__H__

#include <stdint.h>
#include <float.h>
#include <math.h>

#ifndef max
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); \
                    _a > _b ? _a : _b;})
#endif
#ifndef min
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); \
                    _a < _b ? _a : _b;})
#endif

/* To Force C syntax under C++ define ST_USE_C before including */
#ifndef ST_USE_C
#ifdef __cplusplus
#define __ST_USE_CPP_ /* Use C++ syntax */
#endif
#endif /* ST_USE_C */

/* Element type */
#ifdef ST_USE_TYPE
#define __ST_TYPE ST_USE_TYPE
#else
#define __ST_TYPE int64_t
#endif /* ST_USE_TYPE */

#ifndef __UINT8_MAX__
#define __UINT8_MAX__  0xff
#endif
#ifndef __UINT16_MAX__
#define __UINT16_MAX__ 0xffff
#endif
#ifndef __UINT32_MAX__
#define __UINT32_MAX__ 0xffffffffU
#endif
#ifndef __UINT64_MAX__
#define __UINT64_MAX__ 0xffffffffffffffffUL
#endif
#ifndef __INT8_MAX__
#define __INT8_MAX__   0x7f
#endif
#ifndef __INT16_MAX__
#define __INT16_MAX__  0x7fff
#endif
#ifndef __INT32_MAX__
#define __INT32_MAX__  0x7fffffff
#endif
#ifndef __INT64_MAX__
#define __INT64_MAX__  0x7fffffffffffffffL
#endif

#define __ST_COUNT_TYPE uint64_t /* Count type */
#if !defined(__cplusplus) && !defined(__STDC_VERSION__)
/* C89 does not support sqrtl() */
#define __ST_MEAN_TYPE double /* Mean type */
#define __ST_SQRT(a) sqrt(a) /* Use sqrt for long double */
#endif /* ! __cplusplus && ! __STDC_VERSION__ */
#ifndef __ST_MEAN_TYPE
#define __ST_MEAN_TYPE long double /* Mean type */
#endif
#ifndef __ST_SQRT
#define __ST_SQRT(a) sqrtl(a) /* Use sqrt for long double */
#endif
#define __ST_MYSELF nsl_stats

#ifdef __ST_USE_CPP_

#define __ST_NAME(a) a
#define __ST_NAME_GET(a) get_##a
#define __ST_FIELD_(a) a
#define __ST_FIELD_NAME_(a) a
#define __ST_SELF_ void
#define __ST_SELF2_
#define __ST_SELF_VAR_
#define __ST_INLINE_ inline
class __ST_MYSELF
{
private:

#else /*__ST_USE_CPP_*/

#define __ST_NAME(a) nsl_stats_##a
#define __ST_NAME_GET(a) nsl_stats_get_##a
#define __ST_FIELD_(a) st->nsl_stats_##a
#define __ST_FIELD_NAME_(a) nsl_stats_##a
#define __ST_SELF_ struct __ST_MYSELF *st
#define __ST_SELF2_ struct __ST_MYSELF *st,
#define __ST_SELF_VAR_ st
#define __ST_INLINE_ static __inline__
struct __ST_MYSELF
{

#endif /*__ST_USE_CPP_*/

    __ST_TYPE __ST_FIELD_NAME_(min); /* Minimum */
    __ST_TYPE __ST_FIELD_NAME_(max); /* Maximum */

    __ST_COUNT_TYPE __ST_FIELD_NAME_(num); /* Number of elements */

    __ST_MEAN_TYPE __ST_FIELD_NAME_(mean); /* Arithmetic Mean (the average) */
    /* http://mathworld.wolfram.com/ArithmeticMean.html
       http://mathworld.wolfram.com/SampleMean.html */

    __ST_MEAN_TYPE __ST_FIELD_NAME_(sq); /* Arithmetic Mean of X^2 */
    /* Standard Deviation: SD = sqrt ( sq - mean * mean )
       http://mathworld.wolfram.com/StandardDeviation.html */

    /* Mean over limited number of elements (num cut). */
    __ST_COUNT_TYPE __ST_FIELD_NAME_(num_cut); /* number in which we start to cut */
    __ST_MEAN_TYPE __ST_FIELD_NAME_(mean_cut); /* Arithmetic Mean with number cut */
    __ST_MEAN_TYPE __ST_FIELD_NAME_(sq_cut); /* Arithmetic Mean of X^2 with number cut */

#ifdef __ST_USE_CPP_
public:
#else /*__ST_USE_CPP_*/
}; /* struct __ST_MYSELF */
#endif /*__ST_USE_CPP_*/

__ST_INLINE_ void __ST_NAME(add_elem)(__ST_SELF2_ __ST_TYPE e)
{
    register __ST_MEAN_TYPE elem = e;
    register __ST_MEAN_TYPE N = ++(__ST_FIELD_(num));
    __ST_FIELD_(min) = min(__ST_FIELD_(min), e);
    __ST_FIELD_(max) = max(__ST_FIELD_(max), e);
    __ST_FIELD_(mean) += ((__ST_MEAN_TYPE)elem - __ST_FIELD_(mean)) / N;
    __ST_FIELD_(sq) += ((__ST_MEAN_TYPE)elem * elem - __ST_FIELD_(sq)) / N;
    if(__ST_FIELD_(num_cut) < 2)
        return;
    if(N > __ST_FIELD_(num_cut))
    {
        N = __ST_FIELD_(num_cut);
        __ST_FIELD_(mean_cut) +=
            ((__ST_MEAN_TYPE)elem - __ST_FIELD_(mean_cut)) / N;
        __ST_FIELD_(sq_cut) +=
            ((__ST_MEAN_TYPE)elem * elem - __ST_FIELD_(sq_cut)) / N;
    }
    else
    {
        __ST_FIELD_(mean_cut) = __ST_FIELD_(mean);
        __ST_FIELD_(sq_cut) = __ST_FIELD_(sq);
    }
}

__ST_INLINE_ __ST_TYPE __ST_NAME_GET(min)(__ST_SELF_){return __ST_FIELD_(min);}
__ST_INLINE_ __ST_TYPE __ST_NAME_GET(max)(__ST_SELF_){return __ST_FIELD_(max);}
__ST_INLINE_ __ST_COUNT_TYPE __ST_NAME_GET(num_elems)(__ST_SELF_)
{return __ST_FIELD_(num);}
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(mean)(__ST_SELF_)
{return __ST_FIELD_(mean);}
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(sq)(__ST_SELF_)
{return __ST_FIELD_(sq);}
__ST_INLINE_ __ST_COUNT_TYPE __ST_NAME_GET(num_cut)(__ST_SELF_)
{return __ST_FIELD_(num_cut);}
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(mean_cut)(__ST_SELF_)
{return __ST_FIELD_(mean_cut);}
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(sq_cut)(__ST_SELF_)
{return __ST_FIELD_(sq_cut);}

__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(tss)(__ST_SELF_)
{   /* total sum of squares */
    return __ST_FIELD_(sq) - __ST_FIELD_(mean) * __ST_FIELD_(mean);
}
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(variance)(__ST_SELF_)
{   /* variance for standard deviation
       Corrected sample standard deviation (Bessel's correction)
       divide by (N - 1) instead of (N) */
    register __ST_MEAN_TYPE N = __ST_FIELD_(num);
    return __ST_NAME_GET(tss)(__ST_SELF_VAR_) * (N / (N - 1));
}
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(sd)(__ST_SELF_)
{   /* standard deviation for a sample
       Corrected sample standard deviation (Bessel's correction)
       divide by (N - 1) instead of (N) */
    return __ST_SQRT(__ST_NAME_GET(variance)(__ST_SELF_VAR_));
}

__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(tss_cut)(__ST_SELF_)
{
    return __ST_FIELD_(sq_cut) -
        __ST_FIELD_(mean_cut) * __ST_FIELD_(mean_cut);
}
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(variance_cut)(__ST_SELF_)
{
    register __ST_MEAN_TYPE N = __ST_FIELD_(num_cut);
    return __ST_NAME_GET(tss_cut)(__ST_SELF_VAR_) * (N / (N - 1));
}
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(sd_cut)(__ST_SELF_)
{
    return __ST_SQRT(__ST_NAME_GET(variance_cut)(__ST_SELF_VAR_));
}

__ST_INLINE_ void __ST_NAME(init)(__ST_SELF_)
{
    __ST_TYPE m = 1;
    __ST_FIELD_(num) = 0;
    __ST_FIELD_(mean) = __ST_FIELD_(sq) = 0;
    __ST_FIELD_(mean_cut) = __ST_FIELD_(sq_cut) = 0;

    /* Geting the Maximum and minimum depend on element type */
    if(m / 10 > 0) { /* Floating type */
#pragma GCC diagnostic ignored "-Woverflow"
        switch(sizeof(__ST_TYPE)) {
            case sizeof(float):
                __ST_FIELD_(min) = FLT_MAX;
                break;
            case sizeof(double):
                __ST_FIELD_(min) = DBL_MAX;
                break;
            default: /* Must be: long double */
                __ST_FIELD_(min) = LDBL_MAX;
                break;
#pragma GCC diagnostic warning "-Woverflow"
        }
        __ST_FIELD_(max) = -(__ST_FIELD_(min));
    } else if(m - 2 > 0) { /* unsigned Integer type */
        switch(sizeof(__ST_TYPE)) {
            case 1:
                __ST_FIELD_(min) = __UINT8_MAX__;
                break;
            case 2:
                __ST_FIELD_(min) = __UINT16_MAX__;
                break;
            case 4:
                __ST_FIELD_(min) = __UINT32_MAX__;
                break;
            case 8:
                __ST_FIELD_(min) = __UINT64_MAX__;
                break;
            default: /* Must be: 128 bits */
                /* Does not have a macro so calculate the value */
                /* m = 1; */
                __ST_FIELD_(min) = (m + __UINT64_MAX__) * __UINT64_MAX__ + __UINT64_MAX__;
                break;
        }
        __ST_FIELD_(max) = 0;
    } else { /* Signed Integer type */
        switch(sizeof(__ST_TYPE)) {
            case 1:
                __ST_FIELD_(min) = __INT8_MAX__;
                break;
            case 2:
                __ST_FIELD_(min) = __INT16_MAX__;
                break;
            case 4:
                __ST_FIELD_(min) = __INT32_MAX__;
                break;
            case 8:
                __ST_FIELD_(min) = __INT64_MAX__;
                break;
            default: /* Must be: 128 bits */
                /* Does not have a macro so calculate the value */
                /* m = 1; */
                __ST_FIELD_(min) = (m + __UINT64_MAX__) * __INT64_MAX__ + __UINT64_MAX__;
                break;
        }
        __ST_FIELD_(max) = -(__ST_FIELD_(min)) - 1;
    }
}

__ST_INLINE_ void __ST_NAME(set_cut_num)(__ST_SELF2_ __ST_COUNT_TYPE N)
{
    __ST_FIELD_(num_cut) = N;
}

#ifdef __ST_USE_CPP_
    __ST_MYSELF(){__ST_NAME(init)();}
}; /* class __ST_MYSELF */
#endif

#undef __ST_USE_CPP_
#undef __ST_TYPE
#undef __ST_COUNT_TYPE
#undef __ST_MEAN_TYPE
#undef __ST_SQRT
#undef __ST_MYSELF
#undef __ST_NAME
#undef __ST_NAME_GET
#undef __ST_FIELD_
#undef __ST_FIELD_NAME_
#undef __ST_SELF_
#undef __ST_SELF2_
#undef __ST_SELF_VAR_
#undef __ST_INLINE_

#endif/*__NSL_STATISTICS__H__*/
