#pragma once
#define __QO_MEMORY_STREAM_H__

#include "stream_base.h"

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

struct _QO_MemoryStream;
typedef struct _QO_MemoryStream QO_MemoryStream;

void
qo_memstream_open(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        p_buffer ,
    qo_size_t           buffer_size ,
    qo_bool_t           writeable
) QO_NONNULL(1 , 2);

void
qo_memstream_close(
    QO_MemoryStream *   p_memstream
) QO_NONNULL(1);

qo_size_t
qo_memstream_read(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

qo_size_t
qo_memstream_read_at(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

/// @brief Virtually "read" data from the memory stream. It will return 
///        the data from the current position to the end of the stream.
qo_size_t
qo_memstream_ref_read(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t *      p_buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

qo_size_t
qo_memstream_ref_read_at(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t *      p_buffer ,
    qo_size_t           size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

/// @brief Writes data to the memory stream.
/// @note It don't allow write beyond the buffer size.
qo_size_t
qo_memstream_write(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

qo_size_t
qo_memstream_write_at(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

qo_offset_t
qo_memstream_get_pos(
    QO_MemoryStream *   p_memstream 
) QO_NONNULL(1);

qo_offset_t
qo_memstream_seek(
    QO_MemoryStream *               p_memstream ,
    qo_offset_t                     offset ,
    enum QO_FilePointerMoveMethod   move_method ,
    qo_stat_t *                     p_stat
) QO_NONNULL(1);

qo_size_t
qo_memstream_get_size(
    QO_MemoryStream *   p_memstream 
) QO_NONNULL(1);

/// @brief Change the visible size of the memory stream.
/// @remark This call is similar to `qo_abstream_alloc` when abstream warps 
///         over the memory stream.
/// @return The status of the operation.
/// @retval QO_OK The operation was successful.
/// @retval QO_INVALID_ARG The new size is beyond the total buffer size.
qo_stat_t
qo_memstream_resize(
    QO_MemoryStream *   p_memstream ,
    qo_size_t           new_size
) QO_NONNULL(1);

#if defined(__cplusplus)
}
#endif // __cplusplus