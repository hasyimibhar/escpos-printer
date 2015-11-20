#include "error.h"

escpos_error last_error = 0;

escpos_error escpos_last_error()
{
    return last_error;
}
