#include "../../include/abstract_stream.h"
#include <stdio.h>

const extern __QO_AbstractStreamMethods __stdio_abstream_methods;

QO_GLOBAL_LOCAL
qo_stat_t
_abstream_init_by_stdio(
    QO_AbstractStream *     p_abstream ,
    FILE *                  file
) {
    p_abstream->__methods = (__QO_AbstractStreamMethods *)&__memstream_methods;
    p_abstream->__control_block[0] = file;
    return QO_OK;
}

QO_GLOBAL_LOCAL
void
_abstream_stdio_destructor(
    QO_AbstractStream *     p_abstream 
) {
    FILE * file = (FILE *)p_abstream->__control_block[0];
    fclose(file);
    p_abstream->__control_block[0] = NULL;
    p_abstream->__methods = NULL;
}

QO_GLOBAL_LOCAL
qo_size_t
_abstream_stdio_get_size(
    QO_AbstractStream *     p_abstream ,
    qo_stat_t *             p_stat
) {
    
}

QO_GLOBAL_LOCAL
qo_size_t
_abstream_stdio_read(
    QO_AbstractStream *     p_abstream ,
    qo_pointer_t            buffer ,
    qo_size_t               size ,
    qo_stat_t *             p_stat
) {
    FILE * file = (FILE *)p_abstream->__control_block[0];
    qo_size_t read_size = fread(buffer , 1 , size , file);
    if (p_stat)
        *p_stat = (!ferror(file) && !feof(file)) ? QO_OK : QO_UNKNOWN_ERROR;
    return read_size;
}

QO_GLOBAL_LOCAL
qo_size_t
_abstream_stdio_write(
    QO_AbstractStream *     p_abstream ,
    qo_pointer_t            buffer ,
    qo_size_t               size ,
    qo_stat_t *             p_stat
) {
    FILE * file = (FILE *)p_abstream->__control_block[0];
    qo_size_t write_size = fwrite(buffer , 1 , size , file);
    if (p_stat)
        *p_stat = (!ferror(file) && !feof(file)) ? QO_OK : QO_UNKNOWN_ERROR;
    return write_size;
}

QO_GLOBAL_LOCAL
qo_stat_t
_abstream_stdio_remote_read(
    QO_AbstractStream *     p_abstream ,
    qo_pointer_t *          p_buffer ,
    qo_size_t               size ,
    qo_stat_t *             p_stat
) {
    qo_pointer_t buffer = malloc(size);
    if (!buffer)
        return QO_OUT_OF_MEMORY;
    
    FILE * file = (FILE *)p_abstream->__control_block[0];
    qo_size_t read_size = fread(buffer , 1 , size , file);
    if (p_stat)
        *p_stat = (!ferror(file) && !feof(file)) ? QO_OK : QO_UNKNOWN_ERROR;
    *p_buffer = buffer;
    return read_size;
}

QO_GLOBAL_LOCAL
void
_abstream_stdio_remote_unref(
    QO_AbstractStream *     p_abstream ,
    qo_pointer_t            buffer
) {
    free(buffer);
}

QO_PURE
qo_stat_t
__qo_stdio_seeking_error()
{
    switch (errno)
    {
        case ESPIPE:        return QO_BAD_TYPE;
        case EINVAL:        return QO_INVALID_ARG;
        default:            return QO_UNKNOWN_ERROR;
    }
}

QO_GLOBAL_LOCAL
qo_offset_t
_abstream_stdio_seek(
    QO_AbstractStream *             p_abstream ,
    qo_size_t                       offset ,
    QO_FilePointerMoveMethod        move_method ,
    qo_stat_t *                     p_stat
) {
    FILE * file = (FILE *)p_abstream->__control_block[0];
    qo_stat_t stat = fseek(file , offset , move_method) ? __qo_stdio_seeking_error() : QO_OK;
    if (p_stat)
        *p_stat = stat;
    return stat ? -1 : ftell(file);
}

QO_PURE
qo_stat_t
__qo_stdio_telling_error()
{
    switch (errno)
    {
        case EBADF:         return QO_INVALID_HANDLE;
        case ESPIPE:        return QO_BAD_TYPE;
        case EINVAL:        return QO_INVALID_ARG;
        case EIO:           return QO_BROKEN_DEVICE;
        default:            return QO_UNKNOWN_ERROR;
    }
}

QO_GLOBAL_LOCAL
qo_offset_t
_abstream_stdio_get_pos(
    QO_AbstractStream *     p_abstream ,
    qo_stat_t *             p_stat
) {
    FILE * file = (FILE *)p_abstream->__control_block[0];
    qo_stat_t stat = ftell(file) ? __qo_stdio_telling_error() : QO_OK;
    if (p_stat)
        *p_stat = stat;
    return stat ? -1 : ftell(file);
}

QO_PURE
qo_stat_t
__qo_stdio_flushing_error()
{
    switch (errno)
    {
        case EBADF:         return QO_INVALID_HANDLE;
        default:            return QO_UNKNOWN_ERROR;
    }
}

QO_GLOBAL_LOCAL
qo_stat_t
_abstream_stdio_flush(
    QO_AbstractStream *     p_abstream
) {
    FILE * file = (FILE *)p_abstream->__control_block[0];
    int ret = fflush(file);
    return (ret == 0 || ret == EOF) ? QO_OK : __qo_stdio_flushing_error();
}

const __QO_AbstractStreamMethods __stdio_abstream_methods = {
    .destructor     = _abstream_stdio_destructor ,
    .flush          = _abstream_stdio_flush ,
    .get_pos        = _abstream_stdio_get_pos ,
    .read           = _abstream_stdio_read ,
    .read_at        = __unsupport_read_at ,
    .remote_read    = _abstream_stdio_remote_read ,
    .remote_read_at = __unsupport_remote_read_at ,
    .write          = _abstream_stdio_write ,
    .write_at       = __unsupport_write_at ,
    .remote_unref   = _abstream_stdio_remote_unref ,
    .seek           = _abstream_stdio_seek ,
    .get_pos        = _abstream_stdio_get_pos ,
    .alloc          = __unsupport_alloc 
};