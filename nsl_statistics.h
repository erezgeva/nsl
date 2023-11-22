/* SPDX-License-Identifier: MIT
   SPDX-FileCopyrightText: Copyright 2018 Erez Geva */

#ifndef __NSL_STATISTICS__H__
#define __NSL_STATISTICS__H__

/* To Force C syntax under C++ define ST_USE_C before including */
#ifdef __cplusplus
#if __cplusplus >= 201603L /* C++17 */
#define __ST_REGISTER
#endif
#if __cplusplus < 201103L
#include <stdint.h>
#else
#include <cstdint>
#endif
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <cmath>
#ifndef ST_USE_C
#define __ST_USE_CPP_ /* Use C++ syntax */
#endif /* ST_USE_C */
/* use C++ max and min functions */
#include <algorithm>
#define __ST_MAX(a,b) std::max(a,b)
#define __ST_MIN(a,b) std::min(a,b)
#else /* __cplusplus */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#ifndef __STDC_VERSION__
/* C89 does not support sqrtl() */
#define __ST_MEAN_TYPE double /* Mean type */
#define __ST_SQRT(a) sqrt(a) /* Use sqrt for long double */
#endif /* __STDC_VERSION__ */
#endif /* __cplusplus */

#ifndef __ST_REGISTER
#define __ST_REGISTER register
#endif
#define __ST_COUNT_TYPE uint64_t /* Count type */
#ifndef __ST_MEAN_TYPE
#define __ST_MEAN_TYPE long double /* Mean type */
#endif
#ifndef __ST_SQRT
#define __ST_SQRT(a) sqrtl(a) /* Use sqrt for long double */
#endif

#ifndef __ST_MAX
#ifdef max
#define __ST_MAX(a,b) max(a,b)
#else
#define __ST_MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); \
                  _a > _b ? _a : _b;})
#endif /* max */
#endif /* __ST_MAX */

#ifndef __ST_MIN
#ifdef min
#define __ST_MIN(a,b) min(a,b)
#else
#define __ST_MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); \
                    _a < _b ? _a : _b;})
#endif /* min */
#endif /* __ST_MIN */

/* Element type */
#ifdef ST_USE_TYPE
#define __ST_TYPE ST_USE_TYPE
#else
#define __ST_TYPE int64_t
#endif /* ST_USE_TYPE */

#ifndef UINT8_MAX
#define UINT8_MAX  (0xff)
#endif
#ifndef UINT16_MAX
#define UINT16_MAX (0xffff)
#endif
#ifndef UINT32_MAX
#define UINT32_MAX (0xffffffffU)
#endif
#ifndef UINT64_MAX
#define UINT64_MAX (0xffffffffffffffffUL)
#endif
#ifndef INT8_MAX
#define INT8_MAX   (0x7f)
#endif
#ifndef INT16_MAX
#define INT16_MAX  (0x7fff)
#endif
#ifndef INT32_MAX
#define INT32_MAX  (0x7fffffff)
#endif
#ifndef INT64_MAX
#define INT64_MAX  (0x7fffffffffffffffL)
#endif
#ifndef FLT_MAX
#define FLT_MAX    (3.40282346638528859812e+38F)
#endif
#ifndef DBL_MAX
#define DBL_MAX    ((double)1.79769313486231570815e+308L)
#endif
#ifndef LDBL_MAX
#define LDBL_MAX   ((long double)(1.18973149535723176502e+4932L)
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
    /* number in which we start to cut */
    __ST_COUNT_TYPE __ST_FIELD_NAME_(num_cut);
    /* Arithmetic Mean with number cut */
    __ST_MEAN_TYPE __ST_FIELD_NAME_(mean_cut);
    /* Arithmetic Mean of X^2 with number cut */
    __ST_MEAN_TYPE __ST_FIELD_NAME_(sq_cut);

#ifdef __ST_USE_CPP_
public:
#else /*__ST_USE_CPP_*/
}; /* struct __ST_MYSELF */
#endif /*__ST_USE_CPP_*/

__ST_INLINE_ void __ST_NAME(add_elem)(__ST_SELF2_ __ST_TYPE e)
{
    __ST_REGISTER __ST_MEAN_TYPE elem = e;
    __ST_REGISTER __ST_MEAN_TYPE N = ++(__ST_FIELD_(num));
    __ST_FIELD_(min) = __ST_MIN(__ST_FIELD_(min), e);
    __ST_FIELD_(max) = __ST_MAX(__ST_FIELD_(max), e);
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
    __ST_REGISTER __ST_MEAN_TYPE N = __ST_FIELD_(num);
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
    __ST_REGISTER __ST_MEAN_TYPE N = __ST_FIELD_(num_cut);
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
        switch(sizeof(__ST_TYPE)) {
            case sizeof(float):
                __ST_FIELD_(min) = (__ST_TYPE)FLT_MAX;
                break;
            case sizeof(double):
                __ST_FIELD_(min) = (__ST_TYPE)DBL_MAX;
                break;
            default: /* Must be: long double */
                __ST_FIELD_(min) = (__ST_TYPE)LDBL_MAX;
                break;
        }
        __ST_FIELD_(max) = -(__ST_FIELD_(min));
    } else if(m - 2 > 0) { /* unsigned Integer type */
        switch(sizeof(__ST_TYPE)) {
            case 1:
                __ST_FIELD_(min) = (__ST_TYPE)UINT8_MAX;
                break;
            case 2:
                __ST_FIELD_(min) = (__ST_TYPE)UINT16_MAX;
                break;
            case 4:
                __ST_FIELD_(min) = (__ST_TYPE)UINT32_MAX;
                break;
            case 8:
                __ST_FIELD_(min) = (__ST_TYPE)UINT64_MAX;
                break;
            default: /* Must be: 128 bits */
                /* Does not have a macro so calculate the value */
                /* m = 1; */
                __ST_FIELD_(min) = (m + UINT64_MAX) * UINT64_MAX + UINT64_MAX;
                break;
        }
        __ST_FIELD_(max) = 0;
    } else { /* Signed Integer type */
        switch(sizeof(__ST_TYPE)) {
            case 1:
                __ST_FIELD_(min) = (__ST_TYPE)INT8_MAX;
                break;
            case 2:
                __ST_FIELD_(min) = (__ST_TYPE)INT16_MAX;
                break;
            case 4:
                __ST_FIELD_(min) = (__ST_TYPE)INT32_MAX;
                break;
            case 8:
                __ST_FIELD_(min) = (__ST_TYPE)INT64_MAX;
                break;
            default: /* Must be: 128 bits */
                /* Does not have a macro so calculate the value */
                /* m = 1; */
                __ST_FIELD_(min) = (m + UINT64_MAX) * INT64_MAX + UINT64_MAX;
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

/* ****************************************************************** */
/* Statistic vector */
#define __STV_MYSELF nsl_vec_stats

#ifdef __ST_USE_CPP_

#define __STV_NAME(a) a
#define __STV_NAME_GET(a) get_##a
#define __STV_FIELD_(a) a
#define __STV_FIELD_NAME_(a) a
#define __STV_SELF_ void
#define __STV_SELF2_
#define __STV_ME_ this
#define __STV_OTHER __STV_MYSELF *ost
class __STV_MYSELF
{
private:

#else /*__ST_USE_CPP_*/

#define __STV_NAME(a) nsl_vec_stats_##a
#define __STV_NAME_GET(a) nsl_vec_stats_get_##a
#define __STV_FIELD_(a) st->nsl_vec_stats_##a
#define __STV_FIELD_NAME_(a) nsl_vec_stats_##a
#define __STV_SELF_ struct __STV_MYSELF *st
#define __STV_SELF2_ struct __STV_MYSELF *st,
#define __STV_ME_ st
#define __STV_OTHER struct __STV_MYSELF *ost
struct __STV_MYSELF
{

#endif /*__ST_USE_CPP_*/

    /* Vector characters */
    __ST_TYPE __STV_FIELD_NAME_(start); /* start of vector */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(count); /* vector size */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(step); /* step value */
    /* measured values */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(num); /* Number of elements */
    /* Vector for number of elements in range */
    __ST_COUNT_TYPE *__STV_FIELD_NAME_(vector);
    /* Number of elements bellow range */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(below);
    /* Number of elements above range */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(above);

#ifdef __ST_USE_CPP_
public:
#else /*__ST_USE_CPP_*/
}; /* struct __STV_MYSELF */
#endif /*__ST_USE_CPP_*/

__ST_INLINE_ __ST_TYPE __STV_NAME_GET(start)(__STV_SELF_)
{return __STV_FIELD_(start);}
__ST_INLINE_ __ST_TYPE __STV_NAME_GET(end)(__STV_SELF_)
{return __STV_FIELD_(start) + __STV_FIELD_(step) * __STV_FIELD_(count);}
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(step)(__STV_SELF_)
{return __STV_FIELD_(step);}
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(size)(__STV_SELF_)
{return __STV_FIELD_(count);}
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(num)(__STV_SELF_)
{return __STV_FIELD_(num);}
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(below)(__STV_SELF_)
{return __STV_FIELD_(below);}
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(above)(__STV_SELF_)
{return __STV_FIELD_(above);}

__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(value)(__STV_SELF2_ __ST_TYPE e)
{
    __ST_COUNT_TYPE i;
    if(__STV_FIELD_(vector) == NULL)
        return 0;
    if(e < __STV_FIELD_(start))
        return __STV_FIELD_(below);
    i = (e - __STV_FIELD_(start)) / __STV_FIELD_(step);
    if(i < __STV_FIELD_(count))
        return __STV_FIELD_(vector)[i];
    else
        return __STV_FIELD_(above);
}

__ST_INLINE_ void __STV_NAME(add_elem)(__STV_SELF2_ __ST_TYPE e)
{
    __ST_COUNT_TYPE i;
    if(__STV_FIELD_(vector) == NULL)
        return;
    __STV_FIELD_(num)++;
    if(e < __STV_FIELD_(start)) {
        __STV_FIELD_(below)++;
        return;
    }
    i = (e - __STV_FIELD_(start)) / __STV_FIELD_(step);
    if(i < __STV_FIELD_(count))
        __STV_FIELD_(vector)[i]++;
    else
        __STV_FIELD_(above)++;
}

__ST_INLINE_ void __STV_NAME(init)(__STV_SELF2_ __ST_TYPE _start,
        __ST_COUNT_TYPE _count, __ST_COUNT_TYPE _step)
{
    __ST_COUNT_TYPE size;
    if(_step < 1)
        _step = 1; /* Minimum step*/
    __STV_FIELD_(step) = _step;
    if(_count < 1)
        _count = 1; /* Minimum count*/
    __STV_FIELD_(count) = _count;
    __STV_FIELD_(start) = _start;
    __STV_FIELD_(num) = 0;
    __STV_FIELD_(below) = 0;
    __STV_FIELD_(above) = 0;
    size = sizeof(__ST_COUNT_TYPE) * __STV_FIELD_(count);
    __STV_FIELD_(vector) = (__ST_COUNT_TYPE *)malloc(size);
    memset(__STV_FIELD_(vector), 0, size);
}

__ST_INLINE_ void __STV_NAME(remove)(__STV_SELF_)
{
  if(__STV_FIELD_(vector) != NULL) {
      free(__STV_FIELD_(vector));
      __STV_FIELD_(vector) = NULL;
  }
}

__ST_INLINE_ int __STV_NAME(copy)(__STV_SELF2_ const __STV_OTHER)
{
    if(ost != NULL && ost != __STV_ME_)
    {
        __STV_FIELD_(start) = ost->__STV_FIELD_NAME_(start);
        __STV_FIELD_(count) = ost->__STV_FIELD_NAME_(count);
        __STV_FIELD_(step) = ost->__STV_FIELD_NAME_(step);
        __STV_FIELD_(num) = ost->__STV_FIELD_NAME_(num);
        __STV_FIELD_(below) = ost->__STV_FIELD_NAME_(below);
        __STV_FIELD_(above) = ost->__STV_FIELD_NAME_(above);
        if(ost->__STV_FIELD_NAME_(vector) != NULL)
        {
            __ST_COUNT_TYPE size = sizeof(__ST_COUNT_TYPE) *
                                   __STV_FIELD_(count);
            __STV_FIELD_(vector) = (__ST_COUNT_TYPE *)malloc(size);
            if(__STV_FIELD_(vector) != NULL)
                memcpy(__STV_FIELD_(vector), ost->__STV_FIELD_NAME_(vector),
                       size);
        } else
            __STV_FIELD_(vector) = NULL;
        return (1 == 1);
    }
    return (1 == 0);
}

#ifdef __ST_USE_CPP_
    __STV_MYSELF(__ST_TYPE _start = 0, __ST_COUNT_TYPE _count = 1,
            __ST_COUNT_TYPE _step = 1)
        {__STV_NAME(init)(_start, _count, _step);}
    ~__STV_MYSELF(){
        {__STV_NAME(remove)();}
    }
    __STV_MYSELF(const __STV_MYSELF& ost){
        __STV_NAME(copy)((__STV_MYSELF *)&ost);
    }
    __STV_MYSELF& operator=(const __STV_MYSELF& ost)
    {
        if(this != &ost) {
            if(__STV_FIELD_(vector) != NULL)
                free(__STV_FIELD_(vector));
            __STV_NAME(copy)((__STV_MYSELF *)&ost);
        }
        return *this;
    }
}; /* class __ST_MYSELF */
#endif

#undef __ST_USE_CPP_
#undef __ST_REGISTER
#undef __ST_TYPE
#undef __ST_COUNT_TYPE
#undef __ST_MEAN_TYPE
#undef __ST_SQRT
#undef __ST_MAX
#undef __ST_MIN
#undef __ST_MYSELF
#undef __ST_NAME
#undef __ST_NAME_GET
#undef __ST_FIELD_
#undef __ST_FIELD_NAME_
#undef __ST_SELF_
#undef __ST_SELF2_
#undef __ST_SELF_VAR_
#undef __ST_INLINE_
#undef __STV_MYSELF
#undef __STV_NAME
#undef __STV_NAME_GET
#undef __STV_FIELD_
#undef __STV_FIELD_NAME_
#undef __STV_SELF_
#undef __STV_SELF2_
#undef __STV_ME_
#undef __STV_OTHER

#endif/*__NSL_STATISTICS__H__*/
