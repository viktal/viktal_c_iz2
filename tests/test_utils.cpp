#include "gtest/gtest.h"

extern "C" {
#include "emailfilter/utils.h"
}

TEST(utils1, read_to_newline_test)
{
    FILE *file = fopen("199911.txt", "r");
    stream_buffer_state* buffer = create_stream_buffer();
    char* line = NULL;
    char* retcode;

    retcode = fgets(buffer->buffer, buffer->buf_size, file);
    if (retcode == NULL)
        exit(1);
    read_to_newline(file, buffer, &line);
    ASSERT_STREQ(line, "Subject: Changes in Apache mailing lists.");
    free(line);

    retcode = fgets(buffer->buffer, buffer->buf_size, file);
    if (retcode == NULL)
        exit(1);
    read_to_newline(file, buffer, &line);
    ASSERT_STREQ(line, "To: announce@apache.org");
    free(line);

    fclose(file);
    free(buffer->buffer);
    free(buffer);
}

TEST(utils2, skip_to_newline_test)
{
    FILE *file = fopen("199911.txt", "r");
    stream_buffer_state* buffer = create_stream_buffer();
    char* retcode;

    skip_to_new_line(file, buffer);
    ASSERT_STREQ(buffer->buffer, "Subject: Changes in Apache mailing lists.\n");

    retcode = fgets(buffer->buffer, buffer->buf_size, file);
    if (retcode == NULL)
        exit(1);
    ASSERT_STREQ(buffer->buffer, "To: announce@apache.org\n");

    buffer->buf_size = 5;
    retcode = fgets(buffer->buffer, buffer->buf_size, file);
    if (retcode == NULL)
        exit(1);
    skip_to_new_line(file, buffer);
    ASSERT_STREQ(buffer->buffer, "From");

    fclose(file);
    free(buffer->buffer);
    free(buffer);
}
