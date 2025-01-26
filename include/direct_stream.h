#pragma once
#define __QO_DIRECT_STREAM_H__

#include "stream_base.h"

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

struct _QO_DirectStream;
typedef struct _QO_DirectStream QO_DirectStream;

/// @brief  Open a file
/// @param  p_stream A pointer to a file object.
/// @param  path   The path of the file. UTF-8 encoding is required.
/// @param  path_size The size of the path. 0 is allowed, in that case the size 
///         of the path will be calculated automatically.
/// @return The status of the operation.
/// @sa     qo_dfile_close()
qo_stat_t 
qo_dfile_open(
    QO_DirectStream *   p_stream , 
    qo_ccstring_t       path , 
    qo_size_t           path_size ,
    qo_flag32_t         access_mode ,
    qo_flag32_t         open_mode ,
    qo_flag32_t         hints
) QO_NONNULL(1 , 2);

/// @brief  Read data from a file
/// @param  file The file object.
/// @param  buffer A buffer that can hold the data at least `desired_size` bytes.
/// @param  desired_size The size of the data to be read.
/// @param  p_stat A pointer to a status code. If the operation succeeds, the 
///         status code will be set to `QO_OK`. Other error codes may be set.
///         NULL is allowed, in that case the status code will not be set.
/// @return The size of the data that has been read.
qo_size_t 
qo_dfile_read_explicit(
    QO_DirectStream *   p_stream ,
    qo_byte_t *         buffer ,
    qo_size_t           desired_size ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

/// @brief  Read data from a file
/// @param  p_stream The file object.
/// @param  buffer A buffer that can hold the data at least `desired_size` bytes.
/// @param  desired_size The size of the data to be read.
/// @return The size of the data that has been read.
QO_FORCE_INLINE
qo_size_t
qo_dfile_read_implicit(
    QO_DirectStream *   p_stream ,
    qo_byte_t *         buffer ,
    qo_size_t           desired_size
){
    return qo_dfile_read_explicit(p_stream , buffer , desired_size , NULL);
}

/// @brief  Write data to a file at a specific offset
/// @param  file The file object.
/// @param  buffer A buffer that contains the data to be written.
/// @param  desired_size The size of the data to be written.
/// @param  offset The offset of the file.
/// @param  p_stat A pointer to a status code. If the operation succeeds, the
///         status code will be set to `QO_OK`. Other error codes may be set.
///         NULL is allowed, if you don't desire this information.
/// @return The size of the data that has been written. 
/// @sa     qo_dfile_read_at_explicit()
/// @remark In most platforms, parallel reading specified offsets is usually
///         safe despite overlapped regions.
qo_size_t
qo_dfile_read_at_explicit(
    QO_DirectStream *   p_stream ,
    qo_pointer_t        buffer ,
    qo_size_t           desired_size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

/// @brief  Write data to a file at a specific offset
/// @param  file The file object.
/// @param  buffer A buffer that contains the data to be written.
/// @param  desired_size The size of the data to be written.
/// @param  offset The offset of the file.
/// @return The status of the operation.
/// @sa     qo_dfile_write_at_explicit()
// @remark  In most platforms, parallel reading specified offsets is usually
///         safe despite overlapped regions.
QO_FORCE_INLINE
qo_size_t
qo_dfile_read_at_implicit(
    QO_DirectStream *    p_stream ,
    qo_byte_t *         buffer ,
    qo_size_t           desired_size ,
    qo_offset_t         offset
) {
    return qo_dfile_read_at_explicit(
        p_stream , buffer , desired_size , offset , NULL
    );
}

/// @brief  Write data to a file
/// @param  file The file object.
/// @param  buffer A buffer that contains the data to be written.
/// @param  desired_size The size of the data to be written.
/// @param  p_stat A pointer to a status code. If the operation succeeds, the
qo_size_t
qo_dfile_write_explicit(
    QO_DirectStream *    p_stream ,
    qo_byte_t *         buffer ,
    qo_size_t           desired_size ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

/// @brief  Write data to a file
/// @param  file The file object.
/// @param  buffer A buffer that contains the data to be written.
/// @param  desired_size The size of the data to be written.
/// @return The size of the data that has been written.
QO_FORCE_INLINE
qo_size_t
qo_dfile_write_implicit(
    QO_DirectStream *    p_stream ,
    qo_byte_t *         buffer ,
    qo_size_t           desired_size
) {
    return qo_dfile_write_explicit(
        p_stream , buffer , desired_size , NULL
    );
}



/// @brief  Write data to a file at a specific offset
/// @param  file The file object.
/// @param  buffer A buffer that contains the data to be written.
/// @param  desired_size The size of the data to be written.
/// @param  offset The offset of the file.
/// @param  p_stat A pointer to a status code. If the operation succeeds, the
///         status code will be set to `QO_OK`. Other error codes may be set.
///         NULL is allowed, if you don't desire this information.
/// @return The size of the data that has been written.
/// @sa     qo_dfile_write_explicit()
/// @warning Parallel writing with overlapped regions can lead to undefined
///         behavior.
qo_size_t
qo_dfile_write_at_explicit(
    QO_DirectStream *    p_stream ,
    qo_byte_t *         buffer ,
    qo_size_t           desired_size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) QO_NONNULL(1 , 2);

/// @brief  Write data to a file at a specific offset
/// @param  p_stream The file object.
/// @param  buffer A buffer that contains the data to be written.
/// @param  desired_size The size of the data to be written.
/// @param  offset The offset of the file.
/// @return The size of the data that has been written.
/// @sa     qo_dfile_write_at_explicit()
/// @warning Parallel writing with overlapped regions can lead to undefined
///         behavior.
qo_size_t
qo_dfile_write_at_implicit(
    QO_DirectStream *    p_stream ,
    qo_byte_t *         buffer ,
    qo_size_t           desired_size ,
    qo_offset_t         offset
) {
    return qo_dfile_write_at_explicit(
        p_stream , buffer , desired_size , offset , NULL  
    );
}

/// @brief  Close a file
/// @param  file The file object. NULL is allowed, in that case the function
///         will do nothing.
void 
qo_dfile_close(
    QO_DirectStream *    p_stream
);

/// @brief  Allocate a file
/// @param  file The file object.
/// @param  size The size of the file.
/// @return The status of the operation.
qo_stat_t 
qo_dfile_alloc(
    QO_DirectStream *    p_stream ,
    qo_size_t           size
) QO_NONNULL(1);

/// @brief  Move the file pointer.
/// @param  file The file object.
/// @param  offset The offset to move.
/// @param  move_method The method to move the file pointer.
/// @param  p_stat A pointer to a status code. If the operation succeeds, the 
///         status code will be set to `QO_OK`. Other error codes may be set.
///         NULL is allowed, in that case the status code will not be set.
/// @return The offset of the file pointer after the operation.
qo_offset_t 
qo_dfile_seek(
    QO_DirectStream *    p_stream ,
    qo_offset_t         offset ,
    qo_flag32_t         move_method ,
    qo_stat_t *         p_stat
) QO_NONNULL(1);

/// @brief  Get the current position of the file pointer.
/// @param  file The file object.
/// @return 
QO_FORCE_INLINE
qo_offset_t 
qo_dfile_get_position(
    QO_DirectStream *    p_stream ,
    qo_stat_t *         p_stat
) {
    return qo_dfile_seek(p_stream , 0 , QO_FILE_CURRENT , p_stat);
} 

/// @brief  Get the size of a file.
/// @param  file The file object.
/// @param  p_size A pointer to size.
/// @return The status of the operation.
/// @retval QO_OK The operation is successful.
/// @retval QO_
/// @note   Only file or device that supports file-like operations can use this
///         function. 
qo_stat_t 
qo_dfile_get_size(
    QO_DirectStream *   p_stream ,
    qo_size_t *         p_size
) QO_NONNULL(1 , 2);

/// @brief  Get the maximum length of a path.
/// @return The maximum length of a path that XOceanLib can handle.
/// @note   The size is implementation-defined. Usually it is greater than 255.
///         In Windows 1607 or later with long path support enabled, the size
///         is 32767. In early Windows, the size is MAX_PATH, which is defined
///         as 260. In GNU/Linux, the size is 4096.
///         Of note, you may fail to open a file before reaching the maximum
///         length of a path, as many file systems limit a file name to 255
///         characters, which may be smaller than the maximum length.
/// @sa     qo_dfile_open()
QO_PURE
qo_size_t
qo_get_path_max_length();

#if defined(__cplusplus)
}
#endif // __cplusplus

#if QO_PLATFORM(WINDOWS)
#   include "internal/platform_spec/win32/direct_stream.h"
#elif QO_PLATFORM(POSIX)
#   include "internal/platform_spec/posix/direct_stream.h"
#else
#   error "QOLib: File stream interfaces not implemented"
#endif // 

// Make p_stat optional for qo_dfile_read and qo_dfile_write
#define __QO_FILE_RW_SELECT(file , buffer , desired_size , have_operated_size , \
                                __target , ...) __target

#define qo_dfile_read(...)                                               \
        __QO_FILE_RW_SELECT                                             \
        (                                                                   \
            __VA_ARGS__ ,                                                   \
            qo_dfile_read_explicit ,                   \
            qo_dfile_read_implicit                     \
        ) (__VA_ARGS__)

#define qo_dfile_write(...)                                              \
        __QO_FILE_RW_SELECT                                             \
        (                                                                   \ 
            __VA_ARGS__ ,                                                   \
            qo_dfile_read_explicit ,                   \
            qo_dfile_write_implicit                    \
        ) (__VA_ARGS__)