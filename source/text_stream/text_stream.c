#include "../../include/print.h"

typedef struct 
{
    qo_bool_t   highlight;
    qo_bool_t   underline;
} __TextEffectSwitches;

QO_TextStream * __qo_stdout;
QO_TextStream * __qo_stderr;

struct _QO_TextStream
{
    qo_ref_count_t          reference_count;

    void
    (* destructor)(
        QO_TextStream *     self
    );

    QO_AbstractStream *     p_stream;

    QO_IntFormatingMode     int_formating_mode;
    QO_FPFormatingMode      fp32_formating_mode;
    QO_FPFormatingMode      fp64_formating_mode;
    QO_PointerFormatingMode pointer_formating_mode;

    __TextEffectSwitches    effect_switches;

    qo_pointer_t            line_buffer;
    qo_size_t               line_buffer_size;
};

qo_stat_t
__init_stdout_and_stderr()
{
    
}