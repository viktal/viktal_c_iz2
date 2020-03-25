#include "emailfilter/utils.h"

void emailfilter_skip_to_new_line(FILE* file, emailfilterBuffer* stream_buffer) {
    char prev;
    if (ftell(file) == 0) {
        prev = '\n';
    } else {
        fseek(file, (ftell(file) - 1), SEEK_SET);
        prev = getc(file);
    }
    while (prev != '\n') {
        fgets(stream_buffer->buffer, stream_buffer->buf_size, file);
        prev = stream_buffer->buffer[strlen(stream_buffer->buffer) - 1];
    }
    fgets(stream_buffer->buffer, stream_buffer->buf_size, file);
}

emailfilterBuffer* emailfilter_create_buffer(int buf_size) {
    emailfilterBuffer* stream_buffer = (emailfilterBuffer*) malloc(sizeof(emailfilterBuffer));
    if (stream_buffer == NULL)
        exit(EXIT_FAILURE);
    // буфер определенного размера, чтобы не было переполнения, если строка будет слишком большой
    stream_buffer->buf_size = buf_size;
    stream_buffer->buffer = (char*)malloc(stream_buffer->buf_size  * sizeof(char));
    if (stream_buffer->buffer == NULL)
        exit(EXIT_FAILURE);
    return stream_buffer;
}

emailfilterBuffer* emailfilter_create_default_buffer() {
    return emailfilter_create_buffer(1024);
}

void emailfilter_read_to_newline(FILE* file, emailfilterBuffer* stream_buffer, char** result) {
    // buffer already contains newline
    char* newline = strchr(stream_buffer->buffer, '\n');
    if (newline != NULL) {
        int resultlen = newline - stream_buffer->buffer + 1;
        *result = (char *)malloc(resultlen * sizeof(char));
        if (*result == NULL)
            exit(EXIT_FAILURE);
        memcpy(*result, stream_buffer->buffer, resultlen - 1);
        (*result)[resultlen-1] = '\0';
        return;
    }

    // read block-by-block until the newline
    int total_strings = 1;
    char** strings = (char **)malloc(total_strings * sizeof(char*));
    if (strings == NULL)
        exit(EXIT_FAILURE);
    int* strlens = (int*)malloc(total_strings * sizeof(int));
    if (strlens == NULL)
        exit(EXIT_FAILURE);
    strlens[0] = stream_buffer -> buf_size;
    strings[0] = strcpy((char *)malloc(strlens[0] * sizeof(char)), stream_buffer->buffer);
    if (strings[0] == NULL)
        exit(EXIT_FAILURE);

    char lastchar = stream_buffer->buffer[stream_buffer->buf_size - 2];
    size_t real_buf_len = stream_buffer->buf_size;
    while (lastchar != '\n') {
        strings = realloc(strings, (total_strings + 1) * sizeof(char*));
        if (strings == NULL)
            exit(EXIT_FAILURE);
        strlens = realloc(strlens, (total_strings + 1) * sizeof(int));
        if (strlens == NULL)
            exit(EXIT_FAILURE);
        total_strings += 1;

        fgets(stream_buffer->buffer, stream_buffer->buf_size, file);
        real_buf_len = strlen(stream_buffer->buffer) + 1;
        lastchar = stream_buffer->buffer[real_buf_len - 2];
        strlens[total_strings - 1] = real_buf_len;
        strings[total_strings - 1] = memcpy(
                (char *)malloc(real_buf_len * sizeof(char)),
                stream_buffer->buffer, real_buf_len);
        if (strings[total_strings - 1] == NULL)
            exit(EXIT_FAILURE);
    }

    // paste blocks into a single line
    size_t total_length = 0;
    for (int i = 0; i < total_strings; ++i)
        total_length += strlens[i] - 1;
    *result = (char*)malloc((total_length + 1) * sizeof(char));
    if (*result == NULL)
        exit(EXIT_FAILURE);
    size_t curpos = 0;
    for (int i = 0; i < total_strings; ++i) {
        memmove((*result) + curpos, strings[i], strlens[i] - 1);
        curpos += strlens[i] - 1;
        free(strings[i]);
    }
    (*result)[curpos] = '\0';
    free(strlens);
    free(strings);
}
