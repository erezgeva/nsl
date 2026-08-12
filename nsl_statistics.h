/* SPDX-License-Identifier: MIT
   SPDX-FileCopyrightText: Copyright © 2018 Erez Geva <ErezGeva2@gmail.com> */

#ifndef __NSL_STATISTICS__H__
#define __NSL_STATISTICS__H__

#ifdef ST_MEAN_TYPE_DOUBLE
/* Mean type */
#define __ST_MEAN_TYPE double
/* Use sqrt for double */
#define __ST_SQRT(a) sqrt(a)
#endif

#ifdef ST_SKIP_REGISTER
#define __ST_REGISTER
#endif

/* Microsoft C compiler is problematic.
 * It need some work-around. */
#ifdef _MSC_VER
/* The minimum standard for MSVC is C99 */
#ifndef __STDC_VERSION__
#define __STDC_VERSION__ 199901L
#endif
#endif /* _MSC_VER */

/* To Force C syntax under C++ define ST_USE_C before including */
#ifdef __cplusplus
#if __cplusplus >= 201603L /* C++17 */
#ifndef __ST_REGISTER
#define __ST_REGISTER
#endif
#endif /* >= C++17 */
#if __cplusplus < 201103L /* C++11 */
#include <stdint.h>
#ifndef __ST_SQRT
/* Use sqrt for long double */
#define __ST_SQRT(a) sqrt(a)
#endif
#else
#include <cstdint>
#endif
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <cmath>
#ifndef ST_USE_C
/* Use C++ syntax */
#define __ST_USE_CPP_
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
#if !defined __STDC_VERSION__ || __STDC_VERSION__ < 199901L /* C99 */
#ifndef __ST_MEAN_TYPE
/* Mean type */
#define __ST_MEAN_TYPE double
#endif
#ifndef __ST_SQRT
/* Use sqrt for double */
#define __ST_SQRT(a) sqrt(a)
#endif
/* inline was added on C99 */
#define __ST_C_INLINE  __inline__
#endif /* __STDC_VERSION__ */
#endif /* __cplusplus */

#ifndef __ST_REGISTER
#define __ST_REGISTER register
#endif
#ifndef __ST_C_INLINE
#define __ST_C_INLINE  inline
#endif
/* Count type */
#define __ST_COUNT_TYPE uint64_t
#ifndef __ST_MEAN_TYPE
/* Mean type */
#define __ST_MEAN_TYPE long double
#endif
#ifndef __ST_SQRT
/* Use sqrt for long double */
#define __ST_SQRT(a) sqrtl(a)
#endif

#ifndef __ST_MAX
#ifdef max
#define __ST_MAX(a,b) max(a,b)
#elif defined __max
/* Microsoft C compiler always like to complicate */
#define __ST_MAX(a,b) __max(a,b)
#else
#define __ST_MAX(a,b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); \
                  _a > _b ? _a : _b;})
#endif /* max */
#endif /* __ST_MAX */

#ifndef __ST_MIN
#ifdef min
#define __ST_MIN(a,b) min(a,b)
#elif defined __min
/* Microsoft C compiler always like to complicate */
#define __ST_MIN(a,b) __min(a,b)
#else
#define __ST_MIN(a,b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); \
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
/** @file
 * @brief nls classes
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright © 2018 Erez Geva
 *
 */

#define __ST_NAME(a) a
#define __ST_NAME_GET(a) get_##a
#define __ST_FIELD_(a) a
#define __ST_FIELD_NAME_(a) a
#define __ST_SELF_ void
#define __ST_SELF2_
#define __ST_SELF_VAR_
#define __ST_INLINE_ inline
#define __ST_DOXY_SELF
#define __ST_DOXY_SELF_CH

/**
 * Class holding elements statistics
 */
class __ST_MYSELF
{
private:

#else /*__ST_USE_CPP_*/
/** @file
 * @brief nls structures and methods
 *
 * @author Erez Geva <ErezGeva2@@gmail.com>
 * @copyright © 2018 Erez Geva
 *
 */

#define __ST_NAME(a) nsl_stats_##a
#define __ST_NAME_GET(a) nsl_stats_get_##a
#define __ST_FIELD_(a) pstat->nsl_stats_##a
#define __ST_FIELD_NAME_(a) nsl_stats_##a
#define __ST_SELF_ struct __ST_MYSELF *pstat
#define __ST_SELF2_ struct __ST_MYSELF *pstat,
#define __ST_SELF_VAR_ pstat
#define __ST_INLINE_ static __ST_C_INLINE
#define __ST_DOXY_SELF /** @param[in] pstat pointer to statistics structure */
#define __ST_DOXY_SELF_CH /** @param[in,out] pstat pointer to statistics structure */

/**
 * Structure holding elements statistics
 */
struct __ST_MYSELF
{

#endif /*__ST_USE_CPP_*/

    __ST_TYPE __ST_FIELD_NAME_(min); /**< The minimum element value */
    __ST_TYPE __ST_FIELD_NAME_(max); /**< The maximum element value */
    __ST_COUNT_TYPE __ST_FIELD_NAME_(num); /**< Number of elements */
    __ST_MEAN_TYPE __ST_FIELD_NAME_(mean); /**< The Arithmetic Mean (the average) */
    __ST_MEAN_TYPE __ST_FIELD_NAME_(sq); /**< Arithmetic Mean of X^2 */

    /**
     * Mean over limited number of elements (elements cut).
     * number in which we start the elements cut
     */
    __ST_COUNT_TYPE __ST_FIELD_NAME_(num_cut);
    __ST_MEAN_TYPE __ST_FIELD_NAME_(mean_cut); /**< Arithmetic Mean with elements cut */
    __ST_MEAN_TYPE __ST_FIELD_NAME_(sq_cut); /**< Arithmetic Mean of X^2 with elements cut */

#ifdef __ST_USE_CPP_
public:
#else /*__ST_USE_CPP_*/
};
#endif /*__ST_USE_CPP_*/

/**
 * Add element value */
__ST_DOXY_SELF_CH
/**
 * @param[in] element value to add
 */
__ST_INLINE_ void __ST_NAME(add_elem)(__ST_SELF2_ __ST_TYPE element)
{
    __ST_REGISTER __ST_MEAN_TYPE elem = element;
    __ST_REGISTER __ST_MEAN_TYPE N = ++(__ST_FIELD_(num));
    __ST_FIELD_(min) = __ST_MIN(__ST_FIELD_(min), element);
    __ST_FIELD_(max) = __ST_MAX(__ST_FIELD_(max), element);
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
/**
 * Get the minimum element value */
__ST_DOXY_SELF
/**
 * @return the minimum element value
 */
__ST_INLINE_ __ST_TYPE __ST_NAME_GET(min)(__ST_SELF_){return __ST_FIELD_(min);}
/**
 * Get the maximum element value */
__ST_DOXY_SELF
/**
 * @return the maximum element value
 */
__ST_INLINE_ __ST_TYPE __ST_NAME_GET(max)(__ST_SELF_){return __ST_FIELD_(max);}
/**
 * Get number of elements */
__ST_DOXY_SELF
/**
 * @return the number of elements
 */
__ST_INLINE_ __ST_COUNT_TYPE __ST_NAME_GET(num_elems)(__ST_SELF_)
{return __ST_FIELD_(num);}
/**
 * Get Arithmetic Mean */
__ST_DOXY_SELF
/**
 * @return the Arithmetic Mean
 * @note
 *  - http://mathworld.wolfram.com/ArithmeticMean.html
 *  - http://mathworld.wolfram.com/SampleMean.html
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(mean)(__ST_SELF_)
{return __ST_FIELD_(mean);}
/**
 * Get Arithmetic Mean of X^2.
 * Standard Deviation: SD = sqrt ( sq - mean * mean ) */
__ST_DOXY_SELF
/**
 * @return the Arithmetic Mean of X^2
 * @note http://mathworld.wolfram.com/StandardDeviation.html
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(sq)(__ST_SELF_)
{return __ST_FIELD_(sq);}
/**
 * Get number in which we start the cut */
__ST_DOXY_SELF
/**
 * @return the start of the cut
 */
__ST_INLINE_ __ST_COUNT_TYPE __ST_NAME_GET(num_cut)(__ST_SELF_)
{return __ST_FIELD_(num_cut);}
/**
 * Arithmetic Mean with elements cut */
__ST_DOXY_SELF
/**
 * @return the Arithmetic Mean with elements cut
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(mean_cut)(__ST_SELF_)
{return __ST_FIELD_(mean_cut);}
/**
 * Arithmetic Mean of X^2 with elements cut */
__ST_DOXY_SELF
/**
 * @return the Arithmetic Mean of X^2 with elements cut
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(sq_cut)(__ST_SELF_)
{return __ST_FIELD_(sq_cut);}
/**
 * Total sum of squares */
__ST_DOXY_SELF
/**
 * @return sum of squares
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(tss)(__ST_SELF_)
{return __ST_FIELD_(sq) - __ST_FIELD_(mean) * __ST_FIELD_(mean);}
/**
 * Get power of 2 of Variance for standard deviation */
__ST_DOXY_SELF
/**
 * @return power of 2 of Variance for standard deviation
 * @note
 *  Corrected sample standard deviation (Bessel's correction),
 *  divide by (N - 1) instead of (N)
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(variance)(__ST_SELF_)
{
    __ST_REGISTER __ST_MEAN_TYPE N = __ST_FIELD_(num);
    return __ST_NAME_GET(tss)(__ST_SELF_VAR_) * (N / (N - 1));
}
/**
 * Get Variance for standard deviation */
__ST_DOXY_SELF
/**
 * @return Variance for standard deviation
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(sd)(__ST_SELF_)
{return __ST_SQRT(__ST_NAME_GET(variance)(__ST_SELF_VAR_));}
/**
 * Total sum of squares with elements cut */
__ST_DOXY_SELF
/**
 * @return sum of squares with elements cut
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(tss_cut)(__ST_SELF_)
{
    return __ST_FIELD_(sq_cut) -
        __ST_FIELD_(mean_cut) * __ST_FIELD_(mean_cut);
}
/**
 * Get power of 2 of Variance for standard deviation with elements cut */
__ST_DOXY_SELF
/**
 * @return power of 2 of Variance for standard deviation with elements cut
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(variance_cut)(__ST_SELF_)
{
    __ST_REGISTER __ST_MEAN_TYPE N = __ST_FIELD_(num_cut);
    return __ST_NAME_GET(tss_cut)(__ST_SELF_VAR_) * (N / (N - 1));
}
/**
 * Get Variance for standard deviation with elements cut */
__ST_DOXY_SELF
/**
 * @return Variance for standard deviation with elements cut
 */
__ST_INLINE_ __ST_MEAN_TYPE __ST_NAME_GET(sd_cut)(__ST_SELF_)
{
    return __ST_SQRT(__ST_NAME_GET(variance_cut)(__ST_SELF_VAR_));
}
/**
 * Initialize the statistics structure */
__ST_DOXY_SELF_CH
#ifndef __ST_USE_CPP_
/** @note C require active initialization */
#endif /* __ST_USE_CPP_ */
__ST_INLINE_ void __ST_NAME(init)(__ST_SELF_)
{
    __ST_TYPE m = 1;
    __ST_FIELD_(num) = 0;
    __ST_FIELD_(mean) = __ST_FIELD_(sq) = 0;
    __ST_FIELD_(mean_cut) = __ST_FIELD_(sq_cut) = 0;

    /* Getting the Maximum and minimum depend on element type */
    if(m / 2 > 0) { /* Floating type */
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
/**
 * Set the elements cut */
__ST_DOXY_SELF_CH
/**
 * @param[in] number to cut elements up to
 */
__ST_INLINE_ void __ST_NAME(set_cut_num)(__ST_SELF2_ __ST_COUNT_TYPE number)
{
    __ST_FIELD_(num_cut) = number;
}

#ifdef __ST_USE_CPP_
    __ST_MYSELF(){__ST_NAME(init)();}
};
#endif

/* ****************************************************************** */
/* Statistic vector */
#define __STV_MYSELF nsl_vec_stats

#ifdef __ST_USE_CPP_

#define __STV_NAME(a) a
#define __STV_NAME_GET(a) get_##a
#define __STV_FIELD_(a) this->a
#define __STV_FIELD_NAME_(a) a
#define __STV_SELF_ void
#define __STV_SELF2_
#define __STV_ME_ this
#define __STV_OTHER __STV_MYSELF *other
#define __STV_DOXY_SELF
#define __STV_DOXY_SELF_CH

/**
 * Class holding elements belonging to cells of a range
 */
class __STV_MYSELF
{
private:

#else /*__ST_USE_CPP_*/

#define __STV_NAME(a) nsl_vec_stats_##a
#define __STV_NAME_GET(a) nsl_vec_stats_get_##a
#define __STV_FIELD_(a) vector->nsl_vec_stats_##a
#define __STV_FIELD_NAME_(a) nsl_vec_stats_##a
#define __STV_SELF_ struct __STV_MYSELF *vector
#define __STV_SELF2_ struct __STV_MYSELF *vector,
#define __STV_ME_ vector
#define __STV_OTHER struct __STV_MYSELF *other
#define __STV_DOXY_SELF /** @param[in] vector pointer to vector structure */
#define __STV_DOXY_SELF_CH /** @param[in,out] vector pointer to vector structure */

/**
 * Structure holding elements belonging to cells of a range
 */
struct __STV_MYSELF
{

#endif /*__ST_USE_CPP_*/

    /* Vector characters */
    /** The vector value of the lower cell */
    __ST_TYPE __STV_FIELD_NAME_(start);
    /** The number of slots (cells) vector count */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(count);
    /** The vector cell size */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(step);

    /* measured values */
    /** The total number of elements */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(num);
    /** Vector for number of elements in range */
    __ST_COUNT_TYPE *__STV_FIELD_NAME_(vector);
    /** Number of elements bellow range */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(below);
    /** Number of elements above range */
    __ST_COUNT_TYPE __STV_FIELD_NAME_(above);

#ifdef __ST_USE_CPP_
public:
#else /*__ST_USE_CPP_*/
};
#endif /*__ST_USE_CPP_*/

/**
 * Get value of the lower cell, where the vector starts */
__STV_DOXY_SELF
/**
 * @return start storing element values
 */
__ST_INLINE_ __ST_TYPE __STV_NAME_GET(start)(__STV_SELF_)
{return __STV_FIELD_(start);}
/**
 * Get value of the upper cell, where the vector ends */
__STV_DOXY_SELF
/**
 * @return end storing element values
 */
__ST_INLINE_ __ST_TYPE __STV_NAME_GET(end)(__STV_SELF_)
{return __STV_FIELD_(start) + __STV_FIELD_(step) * __STV_FIELD_(count);}
/**
 * Get step value, the size of a cell */
__STV_DOXY_SELF
/**
 * @return step value
 */
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(step)(__STV_SELF_)
{return __STV_FIELD_(step);}
/**
 * Get number of cells (slots) */
__STV_DOXY_SELF
/**
 * @return number of cells
 */
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(size)(__STV_SELF_)
{return __STV_FIELD_(count);}
/**
 * Get total number of elements */
__STV_DOXY_SELF
/**
 * @return total number of elements
 */
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(num)(__STV_SELF_)
{return __STV_FIELD_(num);}
/**
 * Get total number of elements bellow the start value */
__STV_DOXY_SELF
/**
 * @return number of elements bellow the start value
 */
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(below)(__STV_SELF_)
{return __STV_FIELD_(below);}
/**
 * Get total number of elements above the end value */
__STV_DOXY_SELF
/**
 * @return total number of elements above the end value
 */
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(above)(__STV_SELF_)
{return __STV_FIELD_(above);}
/**
 * Get number of elements in cell that belong to the element value */
__STV_DOXY_SELF
/**
 * @param[in] element value
 * @return number of elements in cell
 * @note if element value is bellow start we get number
 *       of elements bellow the start value, same for above end.
 */
__ST_INLINE_ __ST_COUNT_TYPE __STV_NAME_GET(value)(__STV_SELF2_ __ST_TYPE element)
{
    __ST_COUNT_TYPE i;
    if(__STV_FIELD_(vector) == NULL)
        return 0;
    if(element < __STV_FIELD_(start))
        return __STV_FIELD_(below);
    i = (element - __STV_FIELD_(start)) / __STV_FIELD_(step);
    if(i < __STV_FIELD_(count))
        return __STV_FIELD_(vector)[i];
    return __STV_FIELD_(above);
}
/**
 * Add element value to vector */
__STV_DOXY_SELF_CH
/**
 * @param[in] element value to add
 */
__ST_INLINE_ void __STV_NAME(add_elem)(__STV_SELF2_ __ST_TYPE element)
{
    __ST_COUNT_TYPE i;
    if(__STV_FIELD_(vector) == NULL)
        return;
    __STV_FIELD_(num)++;
    if(element < __STV_FIELD_(start)) {
        __STV_FIELD_(below)++;
        return;
    }
    i = (element - __STV_FIELD_(start)) / __STV_FIELD_(step);
    if(i < __STV_FIELD_(count))
        __STV_FIELD_(vector)[i]++;
    else
        __STV_FIELD_(above)++;
}
/**
 * Initialize the vector structure */
__STV_DOXY_SELF_CH
/**
 * @param[in] start value of the vector
 * @param[in] count value of the vector
 * @param[in] step value of the vector
 */
#ifdef __ST_USE_CPP_
/**
 * @note If vector is already initialized,
 *       call remove() before to prevent memory leak.
 */
#else /* __ST_USE_CPP_ */
/**
 * @note C require active initialization
 * @note If vector is already initialized,
 *       call nsl_vec_stats_remove(vector) before to prevent memory leak.
 */
#endif /* __ST_USE_CPP_ */
__ST_INLINE_ void __STV_NAME(init)(__STV_SELF2_ __ST_TYPE start,
        __ST_COUNT_TYPE count, __ST_COUNT_TYPE step)
{
    __ST_COUNT_TYPE size;
    if(step < 1)
        step = 1; /* Minimum step*/
    __STV_FIELD_(step) = step;
    if(count < 1)
        count = 1; /* Minimum count*/
    __STV_FIELD_(count) = count;
    __STV_FIELD_(start) = start;
    __STV_FIELD_(num) = 0;
    __STV_FIELD_(below) = 0;
    __STV_FIELD_(above) = 0;
    size = sizeof(__ST_COUNT_TYPE) * __STV_FIELD_(count);
    __STV_FIELD_(vector) = (__ST_COUNT_TYPE *)malloc(size);
    memset(__STV_FIELD_(vector), 0, size);
}
/**
 * Delete all values in the vector structure */
__STV_DOXY_SELF_CH
__ST_INLINE_ void __STV_NAME(remove)(__STV_SELF_)
{
  if(__STV_FIELD_(vector) != NULL) {
      free(__STV_FIELD_(vector));
      __STV_FIELD_(vector) = NULL;
  }
}
/**
 * Copy vector */
__STV_DOXY_SELF_CH
/**
 * @param[in] other vector to copy
 * @return true on success
 */
__ST_INLINE_ int __STV_NAME(copy)(__STV_SELF2_ const __STV_OTHER)
{
    if(other != NULL && other != __STV_ME_)
    {
        __STV_FIELD_(start) = other->__STV_FIELD_NAME_(start);
        __STV_FIELD_(count) = other->__STV_FIELD_NAME_(count);
        __STV_FIELD_(step) = other->__STV_FIELD_NAME_(step);
        __STV_FIELD_(num) = other->__STV_FIELD_NAME_(num);
        __STV_FIELD_(below) = other->__STV_FIELD_NAME_(below);
        __STV_FIELD_(above) = other->__STV_FIELD_NAME_(above);
        if(other->__STV_FIELD_NAME_(vector) != NULL)
        {
            __ST_COUNT_TYPE size = sizeof(__ST_COUNT_TYPE) *
                                   __STV_FIELD_(count);
            __STV_FIELD_(vector) = (__ST_COUNT_TYPE *)malloc(size);
            if(__STV_FIELD_(vector) != NULL)
                memcpy(__STV_FIELD_(vector), other->__STV_FIELD_NAME_(vector),
                       size);
        } else
            __STV_FIELD_(vector) = NULL;
        return (1 == 1);
    }
    return (1 == 0);
}
#ifdef __ST_USE_CPP_
    /**
     * Constructor
     * @param[in] start value of the vector
     * @param[in] count value of the vector
     * @param[in] step value of the vector
     */
    __STV_MYSELF(__ST_TYPE start = 0, __ST_COUNT_TYPE count = 1,
            __ST_COUNT_TYPE step = 1)
        {__STV_NAME(init)(start, count, step);}
    ~__STV_MYSELF(){
        {__STV_NAME(remove)();}
    }
    /**
     * Copy constructor
     * @param[in] other vector to copy
     */
    __STV_MYSELF(const __STV_MYSELF& other){
        __STV_NAME(copy)((__STV_MYSELF *)&other);
    }
    /**
     * operator equal
     * @param[in] other vector to copy
     */
    __STV_MYSELF& operator=(const __STV_MYSELF& other)
    {
        if(this != &other) {
            if(__STV_FIELD_(vector) != NULL)
                free(__STV_FIELD_(vector));
            __STV_NAME(copy)((__STV_MYSELF *)&other);
        }
        return *this;
    }
};
#endif

#undef __ST_USE_CPP_
#undef __ST_REGISTER
#undef __ST_TYPE
#undef __ST_COUNT_TYPE
#undef __ST_MEAN_TYPE
#undef __ST_SQRT
#undef __ST_C_INLINE
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
#undef __ST_DOXY_SELF
#undef __ST_DOXY_SELF_CH
#undef __STV_MYSELF
#undef __STV_NAME
#undef __STV_NAME_GET
#undef __STV_FIELD_
#undef __STV_FIELD_NAME_
#undef __STV_SELF_
#undef __STV_SELF2_
#undef __STV_ME_
#undef __STV_OTHER
#undef __STV_DOXY_SELF
#undef __STV_DOXY_SELF_CH

#endif/*__NSL_STATISTICS__H__*/
