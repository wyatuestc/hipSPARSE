/* ************************************************************************
 * Copyright 2018 Advanced Micro Devices, Inc.
 *
 * ************************************************************************ */

#pragma once
#ifndef GUARD_HIPSPARSE_MANAGE_PTR
#define GUARD_HIPSPARSE_MANAGE_PTR

#include "arg_check.hpp"

#include <memory>
#include <hip/hip_runtime_api.h>
#include <hipsparse.h>

#define PRINT_IF_HIP_ERROR(INPUT_STATUS_FOR_CHECK)                \
    {                                                             \
        hipError_t TMP_STATUS_FOR_CHECK = INPUT_STATUS_FOR_CHECK; \
        if(TMP_STATUS_FOR_CHECK != hipSuccess)                    \
        {                                                         \
            fprintf(stderr,                                       \
                    "hip error code: %d at %s:%d\n",              \
                    TMP_STATUS_FOR_CHECK,                         \
                    __FILE__,                                     \
                    __LINE__);                                    \
        }                                                         \
    }

namespace hipsparse_test {

// device_malloc wraps hipMalloc and provides same API as malloc
static void* device_malloc(size_t byte_size)
{
    void* pointer;
    PRINT_IF_HIP_ERROR(hipMalloc(&pointer, byte_size));
    return pointer;
}

// device_free wraps hipFree and provides same API as free
static void device_free(void* ptr) { PRINT_IF_HIP_ERROR(hipFree(ptr)); }

struct handle_struct
{
    hipsparseHandle_t handle;
    handle_struct()
    {
        hipsparseStatus_t status = hipsparseCreate(&handle);
        verify_hipsparse_status_success(status, "ERROR: handle_struct constructor");
    }

    ~handle_struct()
    {
        hipsparseStatus_t status = hipsparseDestroy(handle);
        verify_hipsparse_status_success(status, "ERROR: handle_struct destructor");
    }
};

struct descr_struct
{
    hipsparseMatDescr_t descr;
    descr_struct()
    {
        hipsparseStatus_t status = hipsparseCreateMatDescr(&descr);
        verify_hipsparse_status_success(status, "ERROR: descr_struct constructor");
    }

    ~descr_struct()
    {
        hipsparseStatus_t status = hipsparseDestroyMatDescr(descr);
        verify_hipsparse_status_success(status, "ERROR: descr_struct destructor");
    }
};

struct hyb_struct
{
    hipsparseHybMat_t hyb;
    hyb_struct()
    {
        hipsparseStatus_t status = hipsparseCreateHybMat(&hyb);
        verify_hipsparse_status_success(status, "ERROR: hyb_struct constructor");
    }

    ~hyb_struct()
    {
        hipsparseStatus_t status = hipsparseDestroyHybMat(hyb);
        verify_hipsparse_status_success(status, "ERROR: hyb_struct destructor");
    }
};

} // namespace hipsparse_test

using hipsparse_unique_ptr = std::unique_ptr<void, void (*)(void*)>;

#endif // GUARD_HIPSPARSE_MANAGE_PTR