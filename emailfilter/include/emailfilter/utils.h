#pragma once
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

typedef struct stream_buffer_state{
    char *buffer;
    size_t buf_size;
}stream_buffer_state;


stream_buffer_state* create_stream_buffer();
void read_to_newline(FILE* stream, stream_buffer_state* stream_buffer, char** result);
void skip_to_new_line(FILE* file, stream_buffer_state* stream_buffer);
