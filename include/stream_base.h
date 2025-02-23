#pragma once
#define __QO_STREAM_BASE_H__

#define QO_ENABLE_EXPERIMENTAL_CXX
#include "../../../QOZero/include/qozero.h"

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

typedef enum 
{
    QO_FILE_BEGIN = 0,
    QO_FILE_CURRENT = 1,
    QO_FILE_END = 2
} QO_FilePointerMoveMethod;

/* The definition of file access flags are in implements */

#if defined(__cplusplus)
}
#endif // __cplusplus