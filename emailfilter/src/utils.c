#include "emailfilter/utils.h"


void skip_to_new_line(FILE* file, stream_buffer_state* stream_buffer)
{
    char prev;
    if (ftell(file) == 0)
        prev = '\n';
    else {
        fseek(file, (ftell(file) - 1), SEEK_SET);
        prev = getc(file);
    }
    while (prev != '\n'){
        fgets(stream_buffer->buffer, stream_buffer->buf_size, file);
        prev = stream_buffer->buffer[strlen(stream_buffer->buffer) - 1];
    }
    fgets(stream_buffer->buffer, stream_buffer->buf_size, file);
}

stream_buffer_state* create_stream_buffer()
{
    stream_buffer_state* stream_buffer = (stream_buffer_state*) malloc(sizeof(stream_buffer_state));
    stream_buffer->buf_size = 15;  // буфер определенного размера, чтобы не было переполнения,
    // если строка будет слишком большой
    stream_buffer->buffer = (char*)malloc(stream_buffer->buf_size  * sizeof(char));
    return stream_buffer;
}

void read_to_newline(FILE* stream, stream_buffer_state* stream_buffer, char** result)
{
    // buffer already contains newline
    char* newline = strchr(stream_buffer->buffer, '\n');
    if (newline != NULL) {
        int resultlen = newline - stream_buffer->buffer + 1;
        *result = (char *)malloc(resultlen * sizeof(char));
        memcpy(*result, stream_buffer->buffer, resultlen - 1);
        (*result)[resultlen-1] = '\0';
        return;
    }

    // read block-by-block until the newline
    int total_strings = 1;
    char** strings = (char **)malloc(total_strings * sizeof(char*));
    int* strlens = (int*)malloc(total_strings * sizeof(int));
    strlens[0] = stream_buffer -> buf_size;
    strings[0] = strcpy((char *)malloc(strlens[0] * sizeof(char)), stream_buffer->buffer);

    char lastchar = stream_buffer->buffer[stream_buffer->buf_size - 2];
    size_t real_buf_len = stream_buffer->buf_size;
    while (lastchar != '\n')
    {
        strings = realloc(strings, (total_strings + 1) * sizeof(char*));
        strlens = realloc(strlens, (total_strings + 1) * sizeof(int));
        total_strings += 1;

        fgets(stream_buffer->buffer, stream_buffer->buf_size, stream);
        real_buf_len = strlen(stream_buffer->buffer) + 1;
        lastchar = stream_buffer->buffer[real_buf_len - 2];
        strlens[total_strings - 1] = real_buf_len;
        strings[total_strings - 1] = memcpy(
                (char *)malloc(real_buf_len * sizeof(char)),
                stream_buffer->buffer, real_buf_len
        );
    }

    // paste blocks into a single line
    size_t total_length = 0;
    for (int i = 0; i < total_strings; ++i)
        total_length += strlens[i] - 1;
    *result = (char*)malloc((total_length + 1) * sizeof(char));
    size_t curpos = 0;
    for (int i = 0; i < total_strings; ++i)
    {
        memmove((*result) + curpos, strings[i], strlens[i] - 1);
        curpos += strlens[i] - 1;
        free(strings[i]);
    }
    (*result)[curpos] = '\0';
    free(strlens);
    free(strings);
}
