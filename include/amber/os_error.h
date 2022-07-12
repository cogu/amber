#ifndef OS_ERROR_H
#define OS_ERROR_H

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "adt_error.h"

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
typedef int32_t os_error_t;

#define OS_NO_ERROR                   ((os_error_t) 0)
#define OS_INVALID_ARGUMENT_ERROR     ((os_error_t) 1)
#define OS_MEM_ERROR                  ((os_error_t) 2)
#define OS_NOT_IMPLEMENTED_ERROR      ((os_error_t) 3)
#define OS_INTERNAL_ERROR             ((os_error_t) 4)
#define OS_LENGTH_ERROR               ((os_error_t) 5)
#define OS_INDEX_ERROR                ((os_error_t) 6)
#define OS_SYSTIME_ERROR              ((os_error_t) 7)
#define OS_WAIT_ERROR                 ((os_error_t) 8)

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
os_error_t convert_from_adt_to_os_error(adt_error_t error_code);

#endif //OS_ERROR_H
