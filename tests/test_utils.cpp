#include "gtest/gtest.h"

extern "C" {
#include "emailfilter/utils.h"
}

TEST(utils1, read_to_newline_test)
{
    FILE *file = fopen("199911.txt", "r");
    stream_buffer_state* buffer = create_stream_buffer();
    char* line = NULL;
    fgets(buffer->buffer, buffer->buf_size, file);
    read_to_newline(file, buffer, &line);
    ASSERT_STREQ(line, "Subject: Changes in Apache mailing lists.");

    line = NULL;
    buffer->buf_size = 5;
    fgets(buffer->buffer, buffer->buf_size, file);
    read_to_newline(file, buffer, &line);
    ASSERT_STREQ(line, "To: announce@apache.org\n");

    fclose(file);
    free(buffer->buffer);
    free(buffer);
}

TEST(utils2, skip_to_newline_test)
{
    FILE *file = fopen("199911.txt", "r");
    stream_buffer_state* buffer = create_stream_buffer();
    skip_to_new_line(file, buffer);
    ASSERT_STREQ(buffer->buffer, "Subject: Changes in Apache mailing lists.\n");

    fgets(buffer->buffer, buffer->buf_size, file);
    ASSERT_STREQ(buffer->buffer, "To: announce@apache.org\n");

    buffer->buf_size = 5;
    fgets(buffer->buffer, buffer->buf_size, file);
    skip_to_new_line(file, buffer);
    ASSERT_STREQ(buffer->buffer, "From");

    fclose(file);
    free(buffer->buffer);
    free(buffer);
}