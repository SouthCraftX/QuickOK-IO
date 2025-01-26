#include "../../include/abstract_stream.h"

QO_GLOBAL_UNIQUE
qo_size_t
__refuse_read(
    QO_AbstractStream * p_stream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat          
) {
    if (p_stat) 
        *p_stat = QO_PERMISSION_DENIED;
    return 0;
}

QO_GLOBAL_UNIQUE
qo_size_t
__refuse_write(
    QO_AbstractStream * p_stream ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    if (p_stat) 
        *p_stat = QO_PERMISSION_DENIED;
    return 0;
}

QO_GLOBAL_UNIQUE
qo_size_t
__refuse_read_at(
    QO_AbstractStream * p_stream ,
    qo_offset_t            offset ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    if (p_stat) 
        *p_stat = QO_PERMISSION_DENIED;
    return 0;
}

QO_GLOBAL_UNIQUE
qo_size_t
__refuse_write_at(
    QO_AbstractStream * p_stream ,
    qo_offset_t         offset ,
    qo_pointer_t        buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    if (p_stat) 
        *p_stat = QO_PERMISSION_DENIED;
    return 0;
}

QO_GLOBAL_UNIQUE
qo_size_t
__unsupport_get_size(
    QO_AbstractStream * p_stream ,
    qo_stat_t *         p_stat
) {
    if (p_stat) 
        *p_stat = QO_NOT_SUPPORTED;
    return 0;
}

QO_GLOBAL_UNIQUE
qo_stat_t
__unsupport_alloc(
    QO_AbstractStream * p_stream ,
    qo_size_t           size 
) {
    return QO_NOT_SUPPORTED;
}

QO_GLOBAL_UNIQUE
qo_stat_t
__unsupport_seek(
    QO_AbstractStream * p_stream ,
    qo_offset_t         offset ,
    qo_flag32_t         move_method ,
    qo_stat_t *         p_stat
) {
    if (p_stat) 
        *p_stat = QO_NOT_SUPPORTED;
    return QO_NOT_SUPPORTED;
}

QO_GLOBAL_UNIQUE
qo_stat_t
__unsupport_flush(
    QO_AbstractStream * p_stream 
) {
    return QO_NOT_SUPPORTED;
}

QO_GLOBAL_UNIQUE
qo_offset_t
__unsupport_get_pos(
    QO_AbstractStream * p_stream ,
    qo_stat_t *         p_stat
) {
    if (p_stat) 
        *p_stat = QO_NOT_SUPPORTED;
    return 0;
}

QO_GLOBAL_UNIQUE
qo_size_t
__unsupport_write_at(
    QO_AbstractStream * p_stream ,
    qo_offset_t         offset , 
    qo_pointer_t        buffer, 
    qo_size_t           size, 
    qo_stat_t *         p_stat
) {
    if (p_stat) 
        *p_stat = QO_NOT_SUPPORTED;
    return 0;
}

