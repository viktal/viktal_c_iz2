#include "gtest/gtest.h"

extern "C" {
#include "emailfilter/email.h"
}

void free_recepients(emailfilterRecepients* recepients) {
    if (recepients == NULL || recepients->emails == NULL)
        return;
    for (int i = 0; i < recepients->size; i++)
        free(recepients->emails[i]);
    free(recepients->emails);
    recepients->emails = NULL;
    recepients->size = 0;
}

//TEST(email1, mes_end_test) {
//    auto* mes = (emailfilterMessage*) malloc(sizeof(emailfilterMessage));
//    mes->subject = NULL;
//    mes->recepients.emails = NULL;
//    mes->date = NULL;
//    ASSERT_FALSE(emailfilter_is_message_end(mes));
//    char line[] = "subject";
//    size_t size = strlen(line);
//    mes->subject = (char*)malloc(size + 1);
//    memmove(mes->subject, line, size);
//    mes->subject[size] = '\0';
//    ASSERT_FALSE(emailfilter_is_message_end(mes));
//
//    mes->recepients.emails = (char **) malloc(sizeof(char *));
//    mes->recepients.size = 1;
//    mes->recepients.emails[0] = (char *) malloc(size * sizeof(char));
//    memmove(mes->recepients.emails[0], line, size);
//    ASSERT_FALSE(emailfilter_is_message_end(mes));
//
//    mes->date = (emailfilterDate *) malloc(sizeof(emailfilterDate));
//    mes->date->day = 1;
//    mes->date->month = static_cast<emailfilterMonth>(1);
//    mes->date->year = 2000;
//    mes->date->hour = 1;
//    mes->date->minute = 1;
//    mes->date->second = 1;
//    ASSERT_TRUE(emailfilter_is_message_end(mes));
//
//    free(mes);
//}
//
//TEST(email2, parse_mes_test)
//{
//    FILE *file = fopen("199911.txt", "r");
//    emailfilterBuffer* buffer = emailfilter_create_buffer();
//    fgets(buffer->buffer, buffer->buf_size, file);
//    auto* mes = (emailfilterMessage*) malloc(sizeof(emailfilterMessage));
//    mes->subject = NULL;
//    mes->recepients.emails = NULL;
//    mes->date = NULL;
//
//    emailfilter_parse_message(buffer, mes, file);
//    ASSERT_STREQ(mes->subject, "Changes in Apache mailing lists.");
//    ASSERT_FALSE(mes->date);
//    ASSERT_FALSE(mes->recepients.emails);
//    fgets(buffer->buffer, buffer->buf_size, file);
//    emailfilter_parse_message(buffer, mes, file);
//    ASSERT_STREQ(mes->recepients.emails[0], "announce@apache.org");
//    ASSERT_FALSE(mes->date);
//    fgets(buffer->buffer, buffer->buf_size, file);
//    emailfilter_parse_message(buffer, mes, file);
//    ASSERT_EQ(mes->date->day, 4);
//    ASSERT_EQ(mes->date->year, 1999);
//    ASSERT_EQ(mes->date->month, 11);
//
//    fclose(file);
//    free(mes);
//    free(buffer->buffer);
//    free(buffer);
//
//}

TEST(email3, parse_recep_test)
{
    char line1[] = "announce@apache.org";
    auto* recepients = (emailfilterRecepients*)(malloc(sizeof(emailfilterRecepients)));
    recepients->emails = NULL;
    recepients->size = 0;

    emailfilter_parse_recepients(line1, recepients);
    ASSERT_STREQ(recepients->emails[0], line1);
    free_recepients(recepients);

    char line2[] = "announce@apache.org, announce@apache.org";
    emailfilter_parse_recepients(line2, recepients);
    ASSERT_STREQ(recepients->emails[0], "announce@apache.org");
    ASSERT_STREQ(recepients->emails[1], "announce@apache.org");
    free_recepients(recepients);

    char line3[] = "annou <nce@apache> org";
    recepients->emails = NULL;
    emailfilter_parse_recepients(line3, recepients);
//    ASSERT_STREQ(recepients->emails[0], "nce@apache");
    free_recepients(recepients);
//
//    char line4[] = "<announce@apache.org>,announce<@apache.org>";
//    recepients->emails = NULL;
//    emailfilter_parse_recepients(line4, recepients);
////    ASSERT_STREQ(recepients->emails[0], "announce@apache.org");
////    ASSERT_STREQ(recepients->emails[1], "@apache.org");
//    free_recepients(recepients);

    free(recepients);
}

