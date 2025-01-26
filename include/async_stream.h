#pragma once
#define __QO_ASYNC_STREAM_H__

#include "direct_stream.h"

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

struct _QO_AsyncStream;
struct _QO_AsyncIOIndicator;
typedef struct _QO_AsyncStream QO_AsyncStream;
typedef struct _QO_AsyncIOIndicator QO_AsyncIOIndicator;

qo_stat_t
qo_astream_init(
    QO_AsyncStream *    async_stream ,
    QO_DirectStream *   direct_stream ,
    qo_uint32_t         queue_depth ,
    qo_flag32_t         flags
) QO_NONNULL(1 , 2);

void
qo_astream_close(
    QO_AsyncStream *    async_stream
);

qo_stat_t
qo_astream_wait(
    QO_AsyncIOIndicator *   async_io_indicator
) QO_NONNULL(1);

qo_stat_t
qo_astream_wait_all(
    QO_AsyncStream *    async_stream 
) QO_NONNULL(1);

qo_stat_t
qo_astream_is_finished(
    QO_AsyncIOIndicator *   async_io_indicator
) QO_NONNULL(1);

qo_stat_t
qo_astream_submit(
    QO_AsyncStream *        async_stream 
) QO_NONNULL(1);

qo_stat_t
qo_astream_push_read(
    QO_AsyncStream *        async_stream ,
    qo_pointer_t            buffer ,
    qo_size_t               size ,
    QO_AsyncIOIndicator *   async_io_indicator
) QO_NONNULL(1 , 2);

qo_stat_t
qo_astream_push_write(
    QO_AsyncStream *        async_stream ,
    qo_pointer_t            buffer ,
    qo_size_t               size ,
    QO_AsyncIOIndicator *   async_io_indicator
) QO_NONNULL(1 , 2);



// qo_stat_t


#if defined(__cplusplus)
}
#endif // __cplusplus