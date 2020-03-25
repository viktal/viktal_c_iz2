#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct emailfilterBuffer {
    char *buffer;
    size_t buf_size;
}emailfilterBuffer;


emailfilterBuffer* emailfilter_create_buffer(int buf_size);
emailfilterBuffer* emailfilter_create_default_buffer();
void emailfilter_read_to_newline(FILE* file, emailfilterBuffer* stream_buffer, char** result);
void emailfilter_skip_to_new_line(FILE* file, emailfilterBuffer* stream_buffer);
