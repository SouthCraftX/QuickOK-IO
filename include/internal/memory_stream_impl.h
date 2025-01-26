#pragma once
#define __QO_MEMORY_STREAM_IMPL_H__

// for debug
#include "../memory_stream.h"

#if !defined(__QO_MEMORY_STREAM_H__)
#   error Never include this header file directly. Use memory_stream.h instead.
#endif

struct _QO_MemoryStream
{
    qo_byte_t *         buffer;
    qo_byte_t *         current_position;
    qo_size_t           total_size;
    qo_size_t           visible_size;
    qo_bool_t           writeable;
};

QO_GLOBAL_UNIQUE QO_FORCE_INLINE
void
qo_memstream_open(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        p_buffer ,
    qo_size_t           buffer_size ,
    qo_bool_t           writeable
) {
    p_memstream->buffer             = (qo_byte_t *)p_buffer; 
    p_memstream->current_position   = (qo_byte_t *)p_buffer;
    p_memstream->total_size         = buffer_size;
    p_memstream->visible_size       = buffer_size;
    p_memstream->writeable          = writeable;
}

QO_GLOBAL_UNIQUE QO_FORCE_INLINE
void
qo_memstream_close(
    QO_MemoryStream *   p_memstream
) {
    p_memstream->buffer             = NULL;
    p_memstream->current_position   = NULL;
    p_memstream->total_size         = 0;
    p_memstream->visible_size       = 0;
    p_memstream->writeable          = qo_false;
}

QO_GLOBAL_UNIQUE QO_FORCE_INLINE
qo_offset_t
qo_memstream_get_pos(
    QO_MemoryStream *   p_memstream
) {
    return (qo_offset_t)(p_memstream->current_position - p_memstream->buffer);
}

QO_GLOBAL_UNIQUE QO_FORCE_INLINE
qo_size_t
__qo_memstream_op_size(
    QO_MemoryStream *   p_memstream ,
    qo_size_t           desired_size
) {
    qo_size_t operated_size = desired_size;
    if (p_memstream->current_position + desired_size > p_memstream->buffer + p_memstream->visible_size) 
        operated_size = p_memstream->buffer + p_memstream->visible_size - p_memstream->current_position;
    return operated_size;
}

QO_GLOBAL_UNIQUE
qo_size_t
qo_memstream_read(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size
) {
    size = __qo_memstream_op_size(p_memstream , size);
    __builtin_memcpy(buffer , p_memstream->current_position , size); // TODO: switch to qo_memcpy
    p_memstream->current_position += size;
    return size;
}

QO_GLOBAL_UNIQUE
qo_size_t
qo_memstream_write(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {

    if (!p_memstream->writeable) {
        if (p_stat) 
            *p_stat = QO_PERMISSION_DENIED;
        return 0;
    }

    qo_size_t write_size = __qo_memstream_op_size(p_memstream , size);
    if (write_size < size && p_stat) 
        *p_stat =  QO_EOF_MET;

    __builtin_memcpy(p_memstream->current_position, buffer, write_size); // TODO: switch to qo_memcpy
    p_memstream->current_position += write_size;
    return write_size;
}

QO_GLOBAL_UNIQUE
qo_size_t
qo_memstream_write_at(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    if (!p_memstream->writeable) {
        if (p_stat) 
            *p_stat = QO_PERMISSION_DENIED;
        return 0;
    }
    
    qo_byte_t * write_position = p_memstream->buffer + offset;
    qo_size_t write_size = __qo_memstream_op_size(p_memstream , size);
    if (write_size < size && p_stat)
        *p_stat =  QO_EOF_MET;
    
    __builtin_memcpy(write_position, buffer, write_size); // TODO: switch to qo_memcpy
    return write_size;
}

QO_GLOBAL_UNIQUE
qo_size_t
qo_memstream_ref_read(
    QO_MemoryStream *   p_memstream ,
    qo_pointer_t *      p_buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    size = __qo_memstream_op_size(p_memstream , size);
    *p_buffer = p_memstream->current_position;
    p_memstream->current_position += size;
    return size;
}

QO_GLOBAL_UNIQUE QO_FORCE_INLINE
qo_size_t
qo_memstream_get_size(
    QO_MemoryStream *p_memstream
) {
    return p_memstream->visible_size;
}

QO_GLOBAL_UNIQUE
qo_bool_t
__qo_memstream_is_moving_current_bad(
    QO_MemoryStream *   p_memstream ,
    qo_offset_t         offset 
) {
    qo_byte_t * end = p_memstream->buffer + p_memstream->visible_size;
    return (p_memstream->current_position + offset > end || 
            p_memstream->current_position + offset < p_memstream->buffer);
}

QO_GLOBAL_UNIQUE
qo_offset_t
qo_memstream_seek(
    QO_MemoryStream *   p_memstream ,
    qo_offset_t         offset ,
    qo_flag32_t         move_method ,
    qo_stat_t *         p_stat
) {
    qo_byte_t * new_current_position = p_memstream->current_position;
    qo_stat_t   stat        = QO_INVALID_ARG;
    qo_offset_t new_offset  = -1;

    switch (move_method) 
    {
        case QO_FILE_BEGIN:
            if (offset > p_memstream->visible_size || offset < 0) 
                break;
            new_offset = offset;
            new_current_position = p_memstream->buffer + offset;
            stat = QO_OK;
            break;

        case QO_FILE_CURRENT:
            if (__qo_memstream_is_moving_current_bad(p_memstream , offset))
                break;
            new_current_position += offset;
            new_offset = p_memstream->current_position - p_memstream->buffer + offset;
            stat = QO_OK;
            break;

        case QO_FILE_END:
            if (-offset > p_memstream->visible_size || offset > 0)
            {
                new_offset = -1;
                stat = QO_INVALID_ARG;
            }
            new_current_position = p_memstream->buffer + p_memstream->visible_size + offset;
    }

    if (p_stat)
        *p_stat = stat;
    p_memstream->current_position = new_current_position;
    return new_offset;
}

