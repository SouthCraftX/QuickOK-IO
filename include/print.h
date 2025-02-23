#pragma once
#define __QO_PRINT_H__

#include "abstract_stream.h"

struct _QO_TextStream;
typedef struct _QO_TextStream QO_TextStream;

typedef enum
{
    QO_TEXT_STREAM_SUCCEEDED_BYTES = 0 ,
    QO_TEXT_STREAM_SUCCEEDED_CHARS ,
    QO_TEXT_STREAM_ERROR_MESSAGE ,
    QO_TEXT_STREAM_INT_BASE ,
    QO_TEXT_STREAM_FP_BASE ,
    QO_TEXT_STREAM_POINTER_BASE 
} QO_TextStreamInfoPrinter;

typedef enum
{
    QO_TEXT_STREAM_FLUSH 
} QO_TextStreamAction;

typedef enum 
{
    QO_INT_BASE_BINARY,
    QO_INT_BASE_OCTAL,
    QO_INT_BASE_DECIMAL,
    QO_INT_BASE_HEXADECIMAL
} QO_IntFormatingBase;

struct _QO_IntFormatingMode
{
    qo_int8_t           minimum_width; //< Prefix with zeros if necessary
    qo_int8_t           precision;
    struct 
    {
        qo_bool_t       always_show_sign : 1;
        qo_bool_t       show_base_prefix : 1;
        qo_bool_t       uppercase : 1; //< whether prefix and digits are uppercase
    } switches;
    QO_IntFormatingBase base;
};
typedef struct _QO_IntFormatingMode QO_IntFormatingMode;
#define QO_INT_FORMATTING_MODE_DEFAULT \
        (QO_IntFormatingMode) { \
            .base = QO_INT_BASE_DECIMAL , \
            .precision = 0 , \
            .minimum_width = 0 , \
            .switches = { \
                .always_show_sign = qo_false , \
                .show_base_prefix = qo_false , \
                .uppercase = qo_false \
            } \
        }

typedef enum 
{
    QO_FP_SCIENTIFIC_AUTO ,
    QO_FP_SCIENTIFIC_ALWAYS ,
    QO_FP_SCIENTIFIC_NEVER 
} QO_FPScientificMode;

// typedef enum
// {

// } QO_FPRormatingMode;

struct _QO_FPFormatingMode
{
    struct 
    {
        qo_bool_t       always_show_sign : 1;
        qo_bool_t       show_base_prefix : 1;
        qo_bool_t       uppercase : 1; //< whether prefix and digits are uppercase
        qo_bool_t       show_point : 1; //< show '.' in the output
    } switches;
    QO_FPScientificMode scientific_mode;
    QO_IntFormatingBase base;
};
typedef struct _QO_FPFormatingMode QO_FPFormatingMode;
#define QO_FP_FORMATTING_MODE_DEFAULT \
        (QO_FPFormatingMode) { \
            .base = QO_INT_BASE_DECIMAL , \
            .scientific_mode = QO_FP_SCIENTIFIC_AUTO , \
            .switches = { \
                .always_show_sign = qo_false , \
                .show_base_prefix = qo_false , \
                .uppercase = qo_false , \
                .show_point = qo_true \
            } \
        }

struct _QO_PointerFormatingMode
{
    struct
    {
        qo_bool_t       padding : 1; //< pad with front zeroes
        qo_bool_t       uppercase : 1; //< set it false to use lowercase
        qo_bool_t       show_base_prefix : 1; //<  like 0x, 0b, etc.
        qo_bool_t       better_null : 1; //< print NULL as "nullptr"
    } switches;
    QO_IntFormatingBase base;
};
typedef struct _QO_PointerFormatingMode QO_PointerFormatingMode;
#define QO_POINTER_FORMATTING_MODE_DEFAULT \
        (QO_PointerFormatingMode) { \
            .base = QO_INT_BASE_HEXADECIMAL , \
            .switches = { \
                .padding = qo_true , \
                .uppercase = qo_true , \
                .show_base_prefix = qo_true , \
                .better_null = qo_true \
            } \
        }

typedef enum 
{
    QO_TEXT_STREAM_BLACK ,
    QO_TEXT_STREAM_RED ,
    QO_TEXT_STREAM_GREEN ,
    QO_TEXT_STREAM_YELLOW ,
    QO_TEXT_STREAM_BLUE ,
    QO_TEXT_STREAM_PULPLE ,
    QO_TEXT_STREAM_CYAN ,
    QO_TEXT_STREAM_WHITE ,
    QO_TEXT_STREAM_DEFAULT
} QO_TextStreamColor;

typedef enum
{
    QO_TEXT_STREAM_HIGHLIGHT ,
    QO_TEXT_STREAM_UNDERLINE ,
    QO_TEXT_STREAM_BOLD ,
    QO_TEXT_STREAM_BLINK ,
    QO_TEXT_STREAM_REVERSE ,
    QO_TEXT_STREAM_CLEAR ,
    QO_TEXT_STREAM_HIDE_CURSOR ,
    QO_TEXT_STREAM_SHOW_CURSOR
} QO_TextStreamEffect;

struct _QO_TextStreamProperties
{
    qo_bool_t   can_use_colors : 1;
    qo_bool_t   can_use_effects : 1;
    qo_bool_t   can_operate_cursor : 1;
};
typedef struct _QO_TextStreamProperties QO_TextStreamProperties;

QO_TextStream *
qo_get_stdout();

QO_TextStream *
qo_get_stderr();

qo_stat_t
qo_text_stream_get_properties(
    QO_TextStream *             p_text_stream ,
    QO_TextStreamProperties *   p_properties
) QO_NONNULL(1 , 2);

void
qo_print_string(
    QO_TextStream * p_text_stream ,
    qo_ccstring_t   string ,
    qo_size_t       length
) QO_NONNULL(1);

void
qo_print_char(
    QO_TextStream * p_text_stream ,
    qo_int32_t      character
) QO_NONNULL(1);

void
qo_print_i32(
    QO_TextStream * p_text_stream ,
    qo_int32_t      integer
) QO_NONNULL(1);

void
qo_print_u32(
    QO_TextStream * p_text_stream ,
    qo_uint32_t     integer
) QO_NONNULL(1);

void
qo_print_i64(
    QO_TextStream * p_text_stream ,
    qo_int64_t      integer
) QO_NONNULL(1);

void
qo_print_u64(
    QO_TextStream * p_text_stream ,
    qo_uint64_t     integer
) QO_NONNULL(1);

void
qo_print_fp64(
    QO_TextStream * p_text_stream ,
    qo_fp64_t       floating_point
) QO_NONNULL(1);

void
qo_print_fp32(
    QO_TextStream * p_text_stream ,
    qo_fp32_t       floating_point
) QO_NONNULL(1);

void
qo_print_ptr(
    QO_TextStream * p_text_stream ,
    qo_pointer_t    pointer
) QO_NONNULL(1);

void
qo_print_foreground_color(
    QO_TextStream * p_text_stream ,
    QO_TextStreamColor color
) QO_NONNULL(1);

void
qo_print_background_color(
    QO_TextStream * p_text_stream ,
    QO_TextStreamColor color
) QO_NONNULL(1);

void
qo_print_effect(
    QO_TextStream * p_text_stream ,
    QO_TextStreamEffect effect
) QO_NONNULL(1);

void
qo_text_stream_flush(
    QO_TextStream * p_text_stream
) QO_NONNULL(1);

qo_stat_t
qo_text_stream_set_int_formating_mode(
    QO_TextStream *     p_text_stream ,
    QO_IntFormatingMode mode
) QO_NONNULL(1);

QO_IntFormatingMode
qo_text_stream_get_int_formating_mode(
    QO_TextStream *     p_text_stream
) QO_NONNULL(1);

QO_FPFormatingMode
qo_text_stream_get_fp64_formating_mode(
    QO_TextStream *     p_text_stream
) QO_NONNULL(1);

QO_PointerFormatingMode
qo_text_stream_get_pointer_formating_mode(
    QO_TextStream *     p_text_stream
) QO_NONNULL(1);

void
qo_text_stream_set_fp64_formating_mode(
    QO_TextStream *     p_text_stream ,
    QO_FPFormatingMode  mode
) QO_NONNULL(1);

void
qo_text_stream_set_fp32_formating_mode(
    QO_TextStream *     p_text_stream ,
    QO_FPFormatingMode  mode
) QO_NONNULL(1);

void
qo_text_stream_set_all_fp_formating_mode(
    QO_TextStream *     p_text_stream ,
    QO_FPFormatingMode  mode
) QO_NONNULL(1);

void
qo_text_stream_set_pointer_formating_mode(
    QO_TextStream *     p_text_stream ,
    QO_PointerFormatingMode  mode
) QO_NONNULL(1);

void
qo_text_stream_reset_all_formating_mode(
    QO_TextStream *     p_text_stream
) QO_NONNULL(1);


// #if __STDC_VERSION__ >= 201100L
// #define __QO_PRINT_GENERIC__((x) , \
//     qo_ccstring_t : qo_print_string , \
//     qo_int32_t : qo_print_i32 , \
//     qo_uint32_t : qo_print_u32 , \
//     qo_int64_t : qo_print_i64 , \
//     qo_uint64_t : qo_print_u64 , \
//     qo_fp64_t : qo_print_fp64 , \
//     qo_fp32_t : qo_print_fp32 , \
//     qo_pointer_t : qo_print_ptr
//     QO_IntFormatingMode : qo_text_stream_set_all_int_formating_mode , \
//     QO_FPFormatingMode : qo_text_stream_set_all_fp_formating_mode , \
//     QO_PointerFormatingMode : qo_text_stream_set_pointer_formating_mode
// )

// #   if QO_VA_OPT_SUPPORTED
// #       define __QO_WRITELN_FOR_EACH(macro , x) macro(x)
// #       define __QO_WRITELN_FOR_EACH(macro , first , ...) \
//                 macro(first) \
//                 __VA_OPT__(; __QO_WRITELN_FOR_EACH(macro , __VA_ARGS__))
// #       define qo_writeln(...) \
//                 QO_EXPAND(__QO_WRITELN_FOR_EACH(__QO_PRINT_GERICIC__ , __VA_ARGS__))
// #   else
// #       define __QO_WRITELN2(x , ...) \
//                 __QO_PRINT_GERICIC__(x) \
//                 QO_IF_VA_ARGS_NOT_EMPTY(__VA_ARGS__) \
//                     (; QO_EXPAND(__QO_PRINT_GERICIC__(__VA_ARGS__)))
// #   endif

// #endif