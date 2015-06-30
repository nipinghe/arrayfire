/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/dim4.hpp>
#include <af/statistics.h>
#include <af/defines.h>
#include <err_common.hpp>
#include <backend.hpp>
#include <handle.hpp>
#include <reduce.hpp>
#include <arith.hpp>
#include <math.hpp>
#include <cast.hpp>

#include "stats.h"

using namespace detail;

template<typename T>
static T mean(const af_array &in)
{
    /* following function is defined in stats.h */
    return mean<T>(castArray<T>(in)); /* defined in stats.h */
}

template<typename T>
static T mean(const af_array &in, const af_array &weights)
{
    typedef typename baseOutType<T>::type bType;
    /* following function is defined in stats.h */
    return mean<T, bType>(castArray<T>(in), castArray<bType>(weights));
}

template<typename T>
static af_array mean(const af_array &in, const dim_t dim)
{
    /* following function is defined in stats.h */
    return getHandle<T>(mean<T>(castArray<T>(in), dim));
}

template<typename T>
static af_array mean(const af_array &in, const af_array &weights, const dim_t dim)
{
    /* following function is defined in stats.h */
    return getHandle<T>(mean<T>(castArray<T>(in), castArray<T>(weights), dim));
}

af_err af_mean(af_array *out, const af_array in, const dim_t dim)
{
    try {
        ARG_ASSERT(2, (dim>=0 && dim<=3));

        af_array output = 0;
        ArrayInfo info = getInfo(in);
        af_dtype type = info.getType();
        switch(type) {
            case f64: output = mean< double>(in, dim); break;
            case f32: output = mean<  float>(in, dim); break;
            case s32: output = mean<  float>(in, dim); break;
            case u32: output = mean<  float>(in, dim); break;
            case s64: output = mean< double>(in, dim); break;
            case u64: output = mean< double>(in, dim); break;
            case  u8: output = mean<  float>(in, dim); break;
            case  b8: output = mean<  float>(in, dim); break;
            case c32: output = mean< cfloat>(in, dim); break;
            case c64: output = mean<cdouble>(in, dim); break;
            default : TYPE_ERROR(1, type);
        }
        std::swap(*out, output);
    }
    CATCHALL;
    return AF_SUCCESS;
}

af_err af_mean_weighted(af_array *out, const af_array in, const af_array weights, const dim_t dim)
{
    try {
        ARG_ASSERT(2, (dim>=0 && dim<=3));

        af_array output = 0;
        ArrayInfo iInfo = getInfo(in);
        ArrayInfo wInfo = getInfo(weights);
        af_dtype iType  = iInfo.getType();
        af_dtype wType  = wInfo.getType();

        ARG_ASSERT(2, (wType==f32 || wType==f64)); /* verify that weights are non-complex real numbers */

        switch(iType) {
            case f64: output = mean< double>(in, weights, dim); break;
            case f32: output = mean<  float>(in, weights, dim); break;
            case s32: output = mean<  float>(in, weights, dim); break;
            case u32: output = mean<  float>(in, weights, dim); break;
            case s64: output = mean< double>(in, weights, dim); break;
            case u64: output = mean< double>(in, weights, dim); break;
            case  u8: output = mean<  float>(in, weights, dim); break;
            case  b8: output = mean<  float>(in, weights, dim); break;
            case c32: output = mean< cfloat>(in, weights, dim); break;
            case c64: output = mean<cdouble>(in, weights, dim); break;
            default : TYPE_ERROR(1, iType);
        }
        std::swap(*out, output);
    }
    CATCHALL;
    return AF_SUCCESS;
}

af_err af_mean_all(double *realVal, double *imagVal, const af_array in)
{
    try {
        ArrayInfo info = getInfo(in);
        af_dtype type = info.getType();
        switch(type) {
            case f64: *realVal = mean<double>(in); break;
            case f32: *realVal = mean< float>(in); break;
            case s32: *realVal = mean< float>(in); break;
            case u32: *realVal = mean< float>(in); break;
            case s64: *realVal = mean<double>(in); break;
            case u64: *realVal = mean<double>(in); break;
            case  u8: *realVal = mean< float>(in); break;
            case  b8: *realVal = mean< float>(in); break;
            case c32: {
                cfloat tmp = mean<cfloat>(in);
                *realVal = real(tmp);
                *imagVal = imag(tmp);
                } break;
            case c64: {
                cdouble tmp = mean<cdouble>(in);
                *realVal = real(tmp);
                *imagVal = imag(tmp);
                } break;
            default : TYPE_ERROR(1, type);
        }
    }
    CATCHALL;
    return AF_SUCCESS;
}

af_err af_mean_all_weighted(double *realVal, double *imagVal, const af_array in, const af_array weights)
{
    try {
        ArrayInfo iInfo = getInfo(in);
        ArrayInfo wInfo = getInfo(weights);
        af_dtype iType  = iInfo.getType();
        af_dtype wType  = wInfo.getType();

        ARG_ASSERT(3, (wType==f32 || wType==f64)); /* verify that weights are non-complex real numbers */

        switch(iType) {
            case f64: *realVal = mean<double>(in, weights); break;
            case f32: *realVal = mean< float>(in, weights); break;
            case s32: *realVal = mean< float>(in, weights); break;
            case u32: *realVal = mean< float>(in, weights); break;
            case s64: *realVal = mean<double>(in, weights); break;
            case u64: *realVal = mean<double>(in, weights); break;
            case  u8: *realVal = mean< float>(in, weights); break;
            case  b8: *realVal = mean< float>(in, weights); break;
            case c32: {
                cfloat tmp = mean<cfloat>(in, weights);
                *realVal = real(tmp);
                *imagVal = imag(tmp);
                } break;
            case c64: {
                cdouble tmp = mean<cdouble>(in, weights);
                *realVal = real(tmp);
                *imagVal = imag(tmp);
                } break;
            default : TYPE_ERROR(1, iType);
        }
    }
    CATCHALL;
    return AF_SUCCESS;
}
