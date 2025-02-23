#include "general.h"
#include "../../include/memory_stream.h"

const extern __QO_AbstractStreamMethods __memstream_methods;

QO_GLOBAL_LOCAL
qo_stat_t
__abstream_init_by_memstream(
    QO_AbstractStream * p_abstream ,
    QO_MemoryStream *   p_memstream
) {
    p_abstream->__methods = (__QO_AbstractStreamMethods *)&__memstream_methods;
    p_abstream->__control_block[0] = p_memstream;
    // We only use the first element of the control block
    return QO_OK;
}

QO_GLOBAL_LOCAL
void
__abstream_memstream_destructor(
    QO_AbstractStream * p_abstream
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    qo_memstream_close(p_memstream);
    p_abstream->__control_block[0] = NULL;
    p_abstream->__methods = NULL;
}

QO_GLOBAL_LOCAL
qo_size_t
__abstream_memstream_get_size(
    QO_AbstractStream * p_abstream ,
    qo_stat_t *         p_stat
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    qo_size_t size = qo_memstream_get_size(p_memstream);
    if (p_stat) 
        *p_stat = QO_OK;
    return size;
}

QO_GLOBAL_LOCAL
qo_size_t
__abstream_memstream_read(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    return qo_memstream_read(p_memstream , buffer , size , p_stat);
}

QO_GLOBAL_LOCAL
qo_size_t
__abstream_memstream_write(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    return qo_memstream_write(p_memstream , buffer , size , p_stat);
}

QO_GLOBAL_LOCAL
qo_size_t
__abstream_memstream_remote_read(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t *      p_buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    return qo_memstream_ref_read(p_memstream , p_buffer , size , p_stat);
}

QO_GLOBAL_LOCAL
qo_size_t
__abstream_memstream_remote_read_at(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t *      p_buffer ,
    qo_size_t           desired_size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    return qo_memstream_ref_read_at(p_memstream , p_buffer , desired_size , offset , p_stat);
}

QO_GLOBAL_LOCAL
qo_size_t
__abstream_memstream_read_at(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    return qo_memstream_read_at(p_memstream , buffer , size , offset , p_stat);
}

QO_GLOBAL_LOCAL
void
__abstream_memstream_remote_unref(
    QO_AbstractStream * p_abstream ,
    qo_pointer_t        buffer
) { } // We don't need to do anything here

QO_GLOBAL_LOCAL
qo_stat_t
__abstream_memstream_alloc(
    QO_AbstractStream * p_abstream ,
    qo_size_t           size
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    return qo_memstream_resize(p_memstream, size);
}

QO_GLOBAL_LOCAL
qo_offset_t
__abstream_memstream_seek(
    QO_AbstractStream *             p_abstream ,
    qo_offset_t                     offset ,
    QO_FilePointerMoveMethod        move_method ,
    qo_stat_t *                     p_stat
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    return qo_memstream_seek(p_memstream , offset , move_method , p_stat);
}

QO_GLOBAL_LOCAL
qo_offset_t
__abstream_memstream_get_pos(
    QO_AbstractStream * p_abstream ,
    qo_stat_t *         p_stat
) {
    QO_MemoryStream * p_memstream = (QO_MemoryStream *)p_abstream->__control_block[0];
    qo_offset_t offset =  qo_memstream_get_pos(p_memstream);
    if (p_stat) 
        *p_stat = QO_OK;
    return offset;
}

const __QO_AbstractStreamMethods __memstream_methods = {
    .destructor     = __abstream_memstream_destructor,
    .flush          = __unsupport_flush ,
    .get_size       = __abstream_memstream_get_size ,
    .read           = __abstream_memstream_read ,
    .read_at        = __abstream_memstream_read_at ,
    .remote_read    = __abstream_memstream_remote_read ,
    .remote_read_at = __abstream_memstream_remote_read_at ,
    .write          = __abstream_memstream_write ,
    .write_at       = __unsupport_write_at ,
    .remote_unref   = __abstream_memstream_remote_unref ,
    .seek           = __abstream_memstream_seek ,
    .get_pos        = __abstream_memstream_get_pos ,
    .alloc          = __abstream_memstream_alloc  
};