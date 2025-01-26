#pragma once
#define __QO_POSIX_FSTREAM_H__

// for debug
#include "../../../direct_stream.h"

#if !defined(__QO_DIRECT_STREAM_H__)
#   error "Never include this header stream directly. Use <qo/fstream.h> instead."
#endif

#include <unistd.h>
#include <sys/stat.h>
#include "direct_stream_errors.h"

#define QO_FILE_READONLY    O_RDONLY
#define QO_FILE_WRITEONLY   O_WRONLY
#define QO_FILE_READWRITE   O_RDWR

#define QO_FILE_CLEAN_OPEN  ( O_CREAT | O_TRUNC)
#define QO_FILE_OPEN_EXISTING  O_EXCL
#define QO_FILE_TRUNCATE_EXISTING (O_TRUNC | O_EXCL)
#define QO_FILE_NO_BUFFERING  O_DIRECT
#define QO_FILE_SEQUENTIAL  0
#define QO_FILE_RANDOM_ACCESS 0

#define __QO_POSIX_ONCE_READ_LIMIT 0x7ffff000

struct _QO_DirectStream
{
    int fd;
};

QO_GLOBAL_UNIQUE
qo_stat_t 
qo_dstream_open(
    QO_DirectStream *   stream ,
    qo_ccstring_t       path ,
    qo_flag32_t         access_mode ,
    qo_flag32_t         open_mode ,
    qo_flag32_t         hints
){
    const int mode = access_mode | open_mode | hints;
    int fd = open(path , mode);
    if (fd == -1)
    {
        return __qo_dstream_opening_error(); 
    }
    stream->fd = fd;
    return QO_OK;
}

QO_FORCE_INLINE
void 
qo_dstream_close(
    QO_DirectStream * stream
){
    if (stream)
        close(stream->fd);
}

QO_GLOBAL_UNIQUE
qo_size_t
qo_dstream_read(
    QO_DirectStream *   stream ,
    qo_pointer_t        buffer ,
    qo_size_t           desired_size ,
    qo_stat_t *         p_stat
) {
    qo_byte_t * destination = (qo_byte_t *)buffer;
    qo_size_t   total_read = 0;
    qo_stat_t   stat = QO_OK;

    while (total_read < desired_size) 
    {
        qo_size_t target_read = (desired_size - total_read > __QO_POSIX_ONCE_READ_LIMIT) 
                                ? __QO_POSIX_ONCE_READ_LIMIT 
                                : desired_size - total_read;
        qo_ssize_t read_size = read(stream->fd , destination + total_read, target_read);
        if (read_size > 0)
            total_read += read_size;
        else if (read_size == 0)
            break;  // EOF
        else  
        {
            stat = __qo_dstream_reading_error();
            break;
        }
    }

    if (p_stat)
        *p_stat = stat;
    return total_read;
}

QO_GLOBAL_UNIQUE
qo_size_t
qo_dstream_write(
    QO_DirectStream *   stream ,
    qo_pointer_t        buffer ,
    qo_size_t           desired_size ,
    qo_stat_t *         p_stat
) {
    qo_byte_t * source = (qo_byte_t *)buffer;
    qo_size_t   total_written = 0;
    qo_stat_t   stat = QO_OK;

    while (total_written < desired_size)
    {
        qo_size_t target_write = (desired_size - total_written > __QO_POSIX_ONCE_READ_LIMIT) 
                                ? __QO_POSIX_ONCE_READ_LIMIT 
                                : desired_size - total_written;
        qo_ssize_t write_size = write(stream->fd , source + total_written, target_write);
        if (write_size > 0)
            total_written += write_size;    
        else if (write_size == 0)
            break;  // EOF
        else
        {
            stat = __qo_dstream_writing_error();
            break;
        }
    }

    if (p_stat)
        *p_stat = stat;
    return total_written;
}

QO_GLOBAL_UNIQUE
qo_size_t
qo_dstream_read_at(
    QO_DirectStream *   stream ,
    qo_pointer_t        buffer ,
    qo_size_t           desired_size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    qo_byte_t * destination = (qo_byte_t *)buffer;
    qo_size_t   total_read = 0;
    qo_stat_t   stat = QO_OK;

    while (total_read < desired_size)
    {
        qo_size_t target_read = (desired_size - total_read > __QO_POSIX_ONCE_READ_LIMIT)
                                ? __QO_POSIX_ONCE_READ_LIMIT 
                                : desired_size - total_read;    
        qo_ssize_t bytes_read = pread(
            stream->fd , 
            destination + total_read ,
            target_read ,
            offset + total_read
        );
        if (bytes_read > 0) 
            total_read += bytes_read;
        else if (bytes_read == 0)
            break;  // EOF
        else {
            stat = __qo_dstream_reading_error();
            break;
        }
    }

    if (p_stat)
        *p_stat = stat; 
    return total_read;
}

QO_GLOBAL_UNIQUE
qo_size_t
qo_dstream_write_at(
    QO_DirectStream *   stream ,
    qo_pointer_t        buffer ,
    qo_size_t           desired_size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    qo_byte_t * source = (qo_byte_t *)buffer;
    qo_size_t   total_written = 0;
    qo_stat_t   stat = QO_OK;

    while (total_written < desired_size)
    {
        qo_size_t target_write = (desired_size - total_written > __QO_POSIX_ONCE_READ_LIMIT)
                                ? __QO_POSIX_ONCE_READ_LIMIT 
                                : desired_size - total_written;
        qo_ssize_t bytes_written = pwrite(
            stream->fd , 
            source + total_written ,
            target_write ,
            offset + total_written
        );
        if (bytes_written > 0)
            total_written += bytes_written;
        else if (bytes_written == 0)
            break;  // EOF
        else 
        {
            stat = __qo_dstream_writing_error();
            break;
        }
    }

    if (p_stat)
        *p_stat = stat;
    return total_written;
}

QO_GLOBAL_UNIQUE 
qo_size_t
qo_dstream_get_size(
    QO_DirectStream *   stream ,
    qo_stat_t *         p_stat
){
    struct stat file_stat;
    qo_size_t file_size = 0;
    qo_stat_t stat = QO_OK;
    if(fstat(stream->fd , &file_stat))
    {
        stat = __qo_dstream_size_getting_error();
        goto final;
    }
    else if (!S_ISREG(file_stat.st_mode))
    {
        stat = QO_BAD_TYPE;
        goto final;
    }
    file_size = file_stat.st_size;
final:
    if (p_stat)
        *p_stat = stat;
    return file_size;
}

QO_GLOBAL_UNIQUE QO_FORCE_INLINE
qo_offset_t
qo_dstream_seek(
    QO_DirectStream *               stream ,
    qo_offset_t                     desired_offset ,
    enum QO_FilePointerMoveMethod   move_method ,
    qo_stat_t *                     p_stat
){
    off_t off = lseek(stream->fd , desired_offset , move_method);
    if (off == -1 && p_stat)
        *p_stat = __qo_dstream_seeking_error(off);
    return off;
}

QO_GLOBAL_UNIQUE QO_FORCE_INLINE
qo_offset_t
qo_dstream_get_pos(
    QO_DirectStream *   stream ,
    qo_stat_t *         p_stat
) {
    return qo_dstream_seek(stream , 0 , QO_FILE_CURRENT , p_stat);
}

#if QO_PLATFORM(LINUX)
qo_stat_t
qo_dstream_alloc(
    QO_DirectStream *   stream ,
    qo_size_t           size
) {
    return fallocate(stream->fd , 0 , 0 , size) ? __qo_dstream_allocating_error() : QO_OK;
}
#else
qo_stat_t
qo_dstream_alloc(
    QO_DirectStream *   stream ,
    qo_size_t           size
) {
    return ftruncate(stream->fd , size) ? __qo_dstream_allocating_error() : QO_OK;
}
#endif

// QO_FORCE_INLINE
// qo_stat_t 
// qo_fstream_prealloc(
//     QO_DirectStream *    stream , 
//     qo_size_t   size
// ){
//     return fallocate(stream->fd , 0 , 0 , size) ? 
//            __qo_dstream_allocating_error() : QO_OK;
           
// }

// TODO: clean those code for windows.
#if 0

#   include <WinBase.h>

qo_stat_t qo_fstream_prealloc_handle_error()
{
    switch (GetLastError())
    {
        case ERROR_INVALID_HANDLE:
        case ERROR_INVALID_PARAMETER:
        case ERROR_BAD_LENGTH:
        case ERROR_BAD_ARGUMENTS:
        case ERROR_FILE_NOT_FOUND:
        case ERROR_INVALID_HANDLE_STATE:
            return QO_INVALID_ARG;

        case ERROR_NOT_ENOUGH_MEMORY:
            return QO_OUT_OF_MEMORY;

        case ERROR_ACCESS_DENIED:
        case ERROR_SHARING_VIOLATION:
        case ERROR_PRIVILEGE_NOT_HELD:
            return QO_PERMISSION_DENIED;

        case ERROR_BAD_FILE_TYPE:
            return QO_BAD_TYPE;

        case ERROR_FILE_OPERATION_ABORTED:
            return QO_SIGNAL_INTERRUPTED;

        case ERROR_VOLUME_FULL:
        case ERROR_VOLUME_QUOTA_EXCEEDED:
            return QO_DISK_NO_SPACE;

        case ERROR_HANDLE_TIMEOUT:
            return QO_TIMEOUT;

        case ERROR_PIPE_BUSY:
            return QO_BUSY;

        case ERROR_INSUFFICIENT_BUFFER:
            QO_BUG(0);

        default:
            return QO_UNKNOWN;
    }
}

#if 0
qo_stat_t qo_fstream_prealloc(
    QOIOFStream * stream , 
    qo_size_t size
){
    FILE_ALLOCATE_INFORMATION falloc_info;
#   if QO_SYSTEM_BIT(64)
    falloc_info.AllocationSize.QuadPart = size;
#   else
    falloc_info.AllocationSize.LowPart = size;
    falloc_info.AllocationSize.HighPart = 0;
#   endif // QO_SYSTEM_BIT
    return SetFileInformationByHandle((HANDLE)(((QO_DirectStreamStreamBase*)stream)->handle) , 
                               FileAllocationInfo , &falloc_info , 
                               sizeof(falloc_info)) ? QO_OK :
                               qo_fstream_prealloc_handle_error();

}
#endif

#endif // QO_PLATFORM