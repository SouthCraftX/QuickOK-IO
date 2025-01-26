#pragma once
#define __QO_WIN32_DIRECT_STREAM_H__

// for debug
#include "../../../direct_stream.h"

#if !defined(__QO_DIRECT_STREAM_H__)
#   error Never include this header file directly, use <qoean/fstream.h> instead.
#endif // __QO_DIRECT_STREAM_H__

#include <errhandlingapi.h>
#include <fileapi.h>

#define __QO_SINGAL_IO_CALL_LIMIT (1024 * 1024 * 1024)

#define __QO_ACCESS_MODE_PACK(x) (x >> 24)
#define __QO_FLAGS_PACK(x) (x >> 24)

#define __QO_ACCESS_MODE_UNPACK(x) (x << 24)
#define __QO_FLAGS_UNPACK(x) (x << 24)

// `>>24` to make them sustainable for qo_flag8_t.
#define QO_FILE_READ           __QO_ACCESS_MODE_PACK(GENERIC_READ)
#define QO_FILE_WRITE          __QO_ACCESS_MODE_PACK(GENERIC_WRITE)
#define QO_FILE_READ_WRITE     (QO_FILE_READ | QO_FILE_WRITE)

#define QO_FILE_TRUNCATE_EXISTING  TRUNCATE_EXISTING
#define QO_FILE_OPEN_EXISTING      OPEN_EXISTING
#define QO_FILE_CLEAN_OPEN         CREATE_ALWAYS  

// `>>24` to make them sustainable for qo_flag16_t.
#define QO_FILE_SEQUENTIAL      __QO_FLAGS_PACK(FILE_FLAG_SEQUENTIAL_SCAN)
#define QO_FILE_RAMDOM_ACCESS   __QO_FLAGS_PACK(FILE_FLAG_RANDOM_ACCESS)
#define QO_FILE_NO_CACHING      __QO_FLAGS_PACK(FILE_FLAG_NO_BUFFERING)
#define QO_FILE_DELETE_ON_CLOSE __QO_FLAGS_PACK(FILE_FLAG_DELETE_ON_CLOSE)

// Not 32767, to reserve some space for API consumption.
#define __QO_IS_PATH_LEN_UNRESONABLE(path_size) (path_size > 32760)

qo_stat_t
__qo_handle_transcoding_failure()
{
#if QO_DEBUG
    QO_ERRPRINTF("qo_sysfile_open: MultiByteToWideChar failed: ");

    switch (GetLastError())
    {
        // Following 3 errors are technical errors.
        case ERROR_BUFFER_INSUFFICIENT:
            QO_REPORT_BUG("Buffer insufficient.");
        case ERROR_INVALID_FLAGS:
            QO_REPORT_BUG("Invalid flags specified in dwFlags.");
        case ERROR_INVALID_PARAMETER:
            QO_REPORT_BUG("Invalid arg");

        // The only error that can be caused by user input.
        case ERROR_NO_UNICODE_TRANSLATION:
            // QO_REPORT_BUG("Invalid unicode found in string.");
            return QO_BAD_PATH;
    }
#else
    return QO_BAD_PATH;
#endif
}

QO_PURE
qo_stat_t
__qo_sysfile_opening_error()
{
    switch(GetLastError())
    {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
            return QO_BAD_PATH;
        case ERROR_ACCESS_DENIED:
            return QO_PERMISSION_DENIED;
        default:
            return QO_FILE_OPEN_FAILED;
    }
}

QO_PURE
qo_stat_t
__qo_sysfile_reading_error()
{
    switch (GetLastError())
    {
        case ERROR_INVALID_HANDLE:
            return QO_INVALID_HANDLE;
        case ERROR_ACCESS_DENIED:
            return QO_PERMISSION_DENIED;
        // case ERROR_HANDLE_EOF:
        
    }
}

// Extension of Windows's `WriteFile`.  It supports writing more than 4GB.
BOOL
__qo_WriteFile64(
    HANDLE hFile ,
    LPCVOID lpBuffer ,
    ULONG64 dwNumberOfBytesToWrite ,
    PULONG64 lpNumberOfBytesWritten ,
    LPOVERLAPPED lpOverlapped
){
    BOOL ret = TRUE;
    DWORD chuck_size , singly_written;
    OVERLAPPED overlapped;
    
    if (lpOverlapped)
    {
        overlapped = *lpOverlapped;
    }

    do {
        chuck_size = dwNumberOfBytesToWrite > __QO_SINGAL_IO_CALL_LIMIT ? 
                    __QO_SINGAL_IO_CALL_LIMIT : 
                    dwNumberOfBytesToWrite;
        if (!WriteFile(hFile , lpBuffer , chuck_size , &singly_written , &overlapped))
        {
            ret = FALSE;
            break;
        }
        
        if(singly_written < chuck_size)
            break;
        
        lpBuffer = (LPVOID)((qo_byte_t *)lpBuffer + chuck_size);
        dwNumberOfBytesToWrite -= chuck_size;
        *lpNumberOfBytesWritten += singly_written;

        if (lpOverlapped)
            overlapped.Pointer += chuck_size;

    } 
    while(dwNumberOfBytesToWrite);

    return ret;
}

// Extension of Windows's `ReadFile`.  It supports reading more than 4GB.
BOOL
__qo_ReadFile64(
    HANDLE hFile ,
    LPVOID lpBuffer ,
    ULONG64 dwNumberOfBytesToRead ,
    PULONG64 lpNumberOfBytesRead ,
    LPOVERLAPPED lpOverlapped
){
    BOOL ret = TRUE;
    DWORD chuck_size , singly_read;
    OVERLAPPED overlapped;

    if (lpOverlapped)
    {
        overlapped = *lpOverlapped;
    }

    do {
        chuck_size = dwNumberOfBytesToRead > __QO_SINGAL_IO_CALL_LIMIT ? 
                    __QO_SINGAL_IO_CALL_LIMIT : 
                    dwNumberOfBytesToRead;
        if (!ReadFile(hFile , lpBuffer , chuck_size , &singly_read , &overlapped))
        {
            ret = FALSE;
            break;
        }

        if(singly_read < chuck_size)
            break;

        lpBuffer = (LPVOID)((qo_byte_t *)lpBuffer + chuck_size);
        dwNumberOfBytesToRead -= chuck_size;
        *lpNumberOfBytesRead += singly_read;

        if (lpOverlapped)
            overlapped.Pointer += chuck_size;

    }
    while(dwNumberOfBytesToRead);

    return ret;
}

#   if QO_SYSTEM_BIT(64)
qo_size_t qo_sysfile_read64(
    QO_DirectStream *    file ,
    qo_byte_t * buf ,
    qo_ssize_t  size
){
    //
}
#   endif

qo_size_t 
QO_IMPL(qo_sysfile_read_explicit)(
    QO_DirectStream *  file ,
    qo_byte_t *         buffer  ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
) {
    qo_size_t read_size = 0;
    qo_bool_t success = qo_false;
#if QO_SYSTEM_BIT(64)
    if (size > QO_UINT32_MAX) {
        success = __qo_ReadFile64((HANDLE)file, buffer, size, &read_size, NULL);
    } else {
        success = ReadFile((HANDLE)file, buffer, size, (DWORD *)&read_size, NULL);
    }
#else
    success = ReadFile((HANDLE)file, buffer, size, (DWORD *)&read_size, NULL);
#endif

    if (!success) {
        if (p_stat) 
            *p_stat = __qo_sysfile_reading_error();
        return read_size;
    }

    if (p_stat) 
        *p_stat = QO_OK;
    
    return read_size;
}



qo_size_t
QO_IMPL(qo_sysfile_write_explicit)(
    QO_DirectStream *   file ,
    const qo_byte_t *   buffer ,
    qo_size_t           size ,
    qo_stat_t *         p_stat
){
    qo_size_t written_size = 0;
    qo_bool_t success = qo_false;
#if QO_SYSTEM_BIT(64)
    if (size > QO_UINT32_MAX) {
        success = __qo_WriteFile64((HANDLE)file, buffer, size, &written_size, NULL);
    } else {
        success = WriteFile((HANDLE)file, buffer, size, (DWORD *)&written_size, NULL);
    }
#else
    success = WriteFile((HANDLE)file, buffer, size, (DWORD *)&read_size, NULL);
#endif

    if (!success) {
        if (p_stat) 
            *p_stat = __qo_sysfile_writing_error();
        return written_size;
    }

    if (p_stat) 
        *p_stat = QO_OK;
    
    return written_size;
}

qo_size_t 
QO_IMPL(qo_sysfile_read_at_explicit)(
    QO_DirectStream *  file ,
    qo_byte_t *         buffer ,
    qo_size_t           size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    qo_size_t read_size = 0;
    OVERLAPPED overlapped;

#if QO_SYSTEM_BIT(64)
    overlapped.Pointer = offset;
#else
    overlapped.Offset = offset;
    overlapped.OffsetHigh = 0;
#endif

    qo_bool_t success = qo_false;

#if QO_SYSTEM_BIT(64)
    if (size > QO_UINT32_MAX) {
        success = __qo_ReadFile64((HANDLE)file, buffer, size, &read_size, NULL);
    } else {
        success = ReadFile((HANDLE)file, buffer, size, (DWORD *)&read_size, &overlapped);
    }
#else
    read_success = ReadFile((HANDLE)file, buffer, size, (DWORD *)&read_size, &overlapped);
#endif

    if (!success) {
        if (p_stat) 
            *p_stat = __qo_sysfile_reading_error();
        return read_size;
    }

    if (p_stat) 
        *p_stat = QO_OK;
    
    return read_size;
}

qo_size_t 
QO_IMPL(qo_sysfile_write_at_explicit)(
    QO_DirectStream *  file ,
    qo_byte_t *         buffer ,
    qo_size_t           size ,
    qo_offset_t         offset ,
    qo_stat_t *         p_stat
) {
    qo_size_t written_size = 0;
    OVERLAPPED overlapped;

#if QO_SYSTEM_BIT(64)
    overlapped.Pointer = offset;
#else
    overlapped.Offset = offset;
    overlapped.OffsetHigh = 0;
#endif

    qo_bool_t success = qo_false;

#if QO_SYSTEM_BIT(64)
    if (size > QO_UINT32_MAX) {
        success = __qo_WriteFile64((HANDLE)file, buffer, size, &written_size, NULL);
    } else {
        success = WriteFile((HANDLE)file, buffer, size, (DWORD *)&written_size, &overlapped);
    }
#else
    read_success = WriteFile((HANDLE)file, buffer, size, (DWORD *)&read_size, &overlapped);
#endif

    if (!success) {
        if (p_stat) 
            *p_stat = __qo_sysfile_writing_error();
        return written_size;
    }

    if (p_stat) 
        *p_stat = QO_OK;
    
    return written_size;
}


qo_stat_t 
QO_IMPL(qo_sysfile_open)(
    QO_DirectStream ** pp_file , 
    qo_ccstring_t       path , 
    qo_size_t           path_size ,
    qo_flag32_t         access_mode ,
    qo_flag32_t         open_mode ,
    qo_flag32_t         hints
){
    LARGE_INTEGER file_size;
    const qo_size_t long_path_prefix_len = sizeof(L"\\\\?\\") - sizeof(WCHAR);

    if (!path_size) 
        path_size = qo_string_length(path) + sizeof(WCHAR);
    else
        path_size *= sizeof(WCHAR);

    // Check path length here, so reduce the probibility of stack overflow.
    if (__QO_IS_PATH_LEN_UNRESONABLE(path_size))
        return QO_TOO_LONG;
    
    WCHAR wc_path[path_size / 2];

    // Make it a UNC path to support long path.
    if (path_size > MAX_PATH * 2)
    {
        memcpy(wc_path , L"\\\\?\\UNC\\" , long_path_prefix_len);
    }

    const BOOL transcode_status =
    MultiByteToWideChar(
        CP_UTF8 , 0 , path , -1 , 
        wc_path + long_path_prefix_len / 2 , path_size
    );

    if (transcode_status)
    {
        return __qo_handle_transcoding_failure();
    }

    HANDLE file_handle = CreateFileW(
        wc_path , access_mode , 0 , NULL , open_mode , hints , NULL
    );

    if (file_handle == INVALID_HANDLE_VALUE)
    {
        return __qo_sysfile_opening_error();
    }
    *pp_file = (QO_DirectStream *)file_handle;
    return QO_OK;
}

QO_FORCE_INLINE
void qo_sysfile_close(
    QO_DirectStream * p_file
){
    if (file)
        CloseHandle((HANDLE)file);
}

QO_API 
qo_stat_t qo_sysfile_get_size(
    QO_DirectStream * p_file ,
    qo_size_t * p_size
){
#   if QO_SYSTEM_BIT(64)
    BOOL ret = GetFileSizeEx((HANDLE)p_file , (PLARGE_INTEGER)p_size);
#   else
    BOOL ret = GetFileSizeEx((HANDLE)p_file , &(LARGE_INTEGER){.LowPart = *size , .HighPart = 0});
#   endif
    if (ret)
    {
        return QO_OK;
    }
    switch (GetLastError())
    {
        // TODO
        case /* constant-expression */:
            /* code */
            break;

        default:
            break;
    }
}

QO_FORCE_INLINE
qo_offset_t qo_win_large_interger_to_offset(
    LARGE_INTEGER li
){
#   if QO_SYSTEM_BIT(64)
    return li.QuadPart;
#   else
    return li.LowPart;
#   endif 
}

QO_API
qo_offset_t qo_sysfile_seek(
    QO_DirectStream * file ,
    qo_offset_t offset ,
    qo_flag32_t move_method
){
    LARGE_INTEGER after_moving_offset;
#   if QO_SYSTEM_BIT(64)
    LARGE_INTEGER li_offset = {.QuadPart = offset};
#   else
    LARGE_INTEGER li_offset = {.LowPart = offset , .HighPart = 0};
#   endif
    BOOL ret = SetFilePointerEx((HANDLE)file , 
                                li_offset , 
                                &after_moving_offset , 
                                move_method);
    if (ret)
    {
        return qo_win_large_interger_to_offset(after_moving_offset);
    }
    /*switch(GetLastError())
    {
        case ERROR_FILE_NOT_FOUND:
        case ERROR_INVALID_HANDLE:
            return QO_BAD_FILE;
        case ERROR_LOCK_VIOLATION:
            return QO_ACCESS_VIOLATED;
        case ERROR_INVALID_PARAMETER:
            return QO_INVALID_ARG;
        default:
            return QO_UNKNOWN;
    }*/
    return -1;
}
