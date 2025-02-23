#pragma once

// for debug
#include "../../../include/abstract_stream.h"

#if !defined(__QO_ABSTRACT_STREAM_H__)
#   error Never include this header file directly. Use <abstract_stream.h> instead.
#endif

#define __QO_ABSTREAM_CONTROL_BLOCK_ELEMENTS 4

typedef struct 
{
    void
    (* destructor)(
        QO_AbstractStream * self
    );

    qo_size_t
    (* read)(
        QO_AbstractStream * self ,
        qo_pointer_t        buffer ,
        qo_size_t           size ,
        qo_stat_t *         p_stat
    ) QO_NONNULL(1 , 2);

    qo_size_t
    (* write)(
        QO_AbstractStream * self ,
        qo_pointer_t        buffer ,
        qo_size_t           size ,
        qo_stat_t *         p_stat
    );

    qo_size_t
    (* read_at)(
        QO_AbstractStream * self ,
        qo_pointer_t        buffer ,
        qo_size_t           desired_size ,
        qo_offset_t         offset ,
        qo_stat_t *         p_stat
    );

    qo_size_t
    (* write_at)(
        QO_AbstractStream * self ,
        qo_offset_t         offset ,
        qo_pointer_t        buffer ,
        qo_size_t           size ,
        qo_stat_t *         p_stat
    ) ;

    qo_size_t
    (* remote_read)(
        QO_AbstractStream * self ,
        qo_pointer_t *      p_buffer ,
        qo_size_t           size ,
        qo_stat_t *         p_stat
    ) QO_NONNULL(1 , 2);

    qo_size_t
    (* remote_read_at)(
        QO_AbstractStream * self ,
        qo_pointer_t *      p_buffer ,
        qo_size_t           size ,
        qo_offset_t         offset ,
        qo_stat_t *         p_stat
    ) QO_NONNULL(1 , 2);

    void
    (* remote_unref)(
        QO_AbstractStream * self ,
        qo_pointer_t        remote_buffer
    ) QO_NONNULL(1 , 2);

    qo_stat_t
    (* alloc)(
        QO_AbstractStream * self ,
        qo_size_t           size
    );

    qo_offset_t
    (* get_pos)(
        QO_AbstractStream * self ,
        qo_stat_t *         p_stat
    ) QO_NONNULL(1);

    qo_size_t
    (* get_size)(
        QO_AbstractStream * self ,
        qo_stat_t *         p_stat
    ) QO_NONNULL(1);

    qo_offset_t
    (* seek)(
        QO_AbstractStream *             self ,
        qo_offset_t                     offset ,
        QO_FilePointerMoveMethod   move_method ,
        qo_stat_t *                     p_stat
    ) QO_NONNULL(1);

    qo_stat_t
    (* flush)(
        QO_AbstractStream * self
    ) QO_NONNULL(1);
} __QO_AbstractStreamMethods;

struct _QO_AbstractStream
{
    __QO_AbstractStreamMethods *    __methods;
    qo_pointer_t                    __control_block[__QO_ABSTREAM_CONTROL_BLOCK_ELEMENTS];
};

QO_FORCE_INLINE
void
qo_abstream_unref(
    QO_AbstractStream * p_abstream
) {
    if (p_abstream) {
        p_abstream->__methods->destructor(p_abstream);
    }
}

QO_FORCE_INLINE
qo_size_t
qo_abstream_read(
    QO_AbstractStream * self ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    return self->__methods->read(self , buffer , size , p_stat);
}
    
QO_FORCE_INLINE
qo_size_t
qo_abstream_write(
    QO_AbstractStream * self ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    return self->__methods->write(self , buffer , size , p_stat);
}

QO_FORCE_INLINE
qo_size_t
qo_abstream_read_at(
    QO_AbstractStream * self ,
    qo_pointer_t        buffer ,
    qo_size_t           buffer_size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    return self->__methods->read_at(self , buffer , buffer_size , offset , p_stat);
}

QO_FORCE_INLINE
qo_size_t
qo_abstream_write_at(
    QO_AbstractStream * self ,
    qo_offset_t         offset ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    return self->__methods->write_at(self , offset , buffer , size , p_stat);
}

QO_FORCE_INLINE
qo_size_t
qo_abstream_remote_read(
    QO_AbstractStream * self ,
    qo_pointer_t *      p_buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    return self->__methods->remote_read(self , p_buffer , size , p_stat);
}

QO_FORCE_INLINE
qo_size_t
qo_abstream_remote_read_at(
    QO_AbstractStream * self ,
    qo_pointer_t *      p_buffer ,
    qo_size_t           size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    return self->__methods->remote_read_at(self , p_buffer , size , offset , p_stat);
}

QO_FORCE_INLINE
qo_stat_t
qo_abstream_alloc(
    QO_AbstractStream * self ,
    qo_size_t           size
) {
    return self->__methods->alloc(self , size);
}

QO_FORCE_INLINE
qo_offset_t
qo_abstream_get_pos(
    QO_AbstractStream * self ,
    qo_stat_t *         p_stat
) {
    return self->__methods->get_pos(self , p_stat);
}

QO_FORCE_INLINE
qo_offset_t
qo_abstream_seek(
    QO_AbstractStream *             self ,
    qo_offset_t                     offset ,
    QO_FilePointerMoveMethod   move_method ,
    qo_stat_t *                     p_stat
) {
    return self->__methods->seek(self , offset , move_method , p_stat);
}

QO_FORCE_INLINE
qo_stat_t
qo_abstream_flush(
    QO_AbstractStream * self
) {
    return self->__methods->flush(self);
}

QO_FORCE_INLINE
qo_size_t
qo_abstream_get_size(
    QO_AbstractStream * self ,
    qo_stat_t *         p_stat
) {
    return self->__methods->get_size(self , p_stat);
}

