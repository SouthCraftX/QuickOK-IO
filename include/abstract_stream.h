#pragma once
#include "stream_base.h"
#define __QO_ABSTRACT_STREAM_H__

#include "direct_stream.h"
#include "memory_stream.h"

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

struct _QO_AbstractStream;
typedef struct _QO_AbstractStream QO_AbstractStream;

qo_stat_t
qo_abstream_from_dstream(
    QO_AbstractStream * p_abstream ,
    QO_DirectStream *   p_dstream
) QO_NONNULL(1);

qo_stat_t
qo_abstream_from_memstream(
    QO_MemoryStream *   p_memstream
) QO_NONNULL(1);

void
qo_abstream_close(
    QO_AbstractStream * p_abstream
);

qo_size_t
qo_abstream_read(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t        buffer ,
    qo_size_t           buffer_size , 
    qo_size_t *         p_stat
) QO_NONNULL(1 , 2);

qo_size_t
qo_abstream_write(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t        buffer ,
    qo_size_t           buffer_size ,
    qo_size_t *         p_stat
) QO_NONNULL(1 , 2);

qo_stat_t
qo_abstream_remote_unref(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t        remote_buffer 
) QO_NONNULL(1 , 2);

/// @brief Read data and put it into a remote buffer.
/// @note  Remote buffer pointed by remote_buffer must be freed by 
///        qo_abstream_remote_unref() after use.
qo_size_t
qo_abstream_remote_read(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t *      p_buffer ,
    qo_size_t *         buffer_size ,
    qo_size_t *         p_stat
) QO_NONNULL(1 , 2 , 3);

qo_size_t
qo_abstream_remote_read_at(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t *      p_buffer ,
    qo_size_t *         buffer_size ,
    qo_offset_t         offset ,
    qo_size_t *         p_stat
) QO_NONNULL(1 , 3 , 4);

qo_size_t
qo_abstream_read_at(
    QO_AbstractStream * p_abstream ,
    qo_offset_t         offset ,
    qo_pointer_t        buffer ,
    qo_size_t           buffer_size ,
    qo_size_t *         p_stat
) QO_NONNULL(1 , 3);

qo_size_t
qo_abstream_write_at(
    QO_AbstractStream * p_abstream ,
    qo_offset_t         offset ,
    qo_pointer_t        buffer ,
    qo_size_t           buffer_size ,
    qo_size_t *         p_stat
) QO_NONNULL(1 , 3);

qo_offset_t
qo_abstream_seek(
    QO_AbstractStream *             p_abstream ,
    qo_offset_t                     offset ,
    enum QO_FilePointerMoveMethod   move_method ,
    qo_offset_t *                   p_stat
) QO_NONNULL(1);

qo_size_t
qo_abstream_get_size(
    QO_AbstractStream * p_abstream ,
    qo_size_t *         p_stat
) QO_NONNULL(1 , 2);

qo_offset_t
qo_abstream_get_pos(
    QO_AbstractStream * p_abstream ,
    qo_offset_t *       p_stat
) QO_NONNULL(1 , 2);

qo_stat_t
qo_abstream_flush(
    QO_AbstractStream * p_abstream
) QO_NONNULL(1);

qo_stat_t
qo_abstream_alloc(
    QO_AbstractStream * p_abstream ,
    qo_size_t           size 
) QO_NONNULL(1);

#if defined(__cplusplus)
}
#endif // __cplusplus

#include "internal/abstract_stream_impls/general.h"