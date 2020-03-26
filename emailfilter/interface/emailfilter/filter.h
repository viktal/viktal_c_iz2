#pragma once
#include <stdio.h>
#include <stdint.h>
#include "emailfilter/email.h"
#include "emailfilter/date.h"

typedef struct emailfilterParams {
    emailfilterDate* begin;
    emailfilterDate* end;
    char* recepient;
}emailfilterParams;

typedef struct emailfilterResult {
    emailfilterMessage** emails;
    size_t size;
}emailfilterResult;

emailfilterResult* emailfilter_filter(const char* path, emailfilterParams* params, uint16_t num_threads);
