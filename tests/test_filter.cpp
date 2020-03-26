#include "gtest/gtest.h"

extern "C" {
#include "emailfilter/filter.h"
}

class test_fixture_for_filter: public ::testing::Test {
public:

    emailfilterParams params {};
    std::string path = "201105.txt";

    void check_results(emailfilterResult* result) {
        ASSERT_STREQ(result->emails[0]->subject, "Re: [ANNOUNCEMENT] Apache Gora 0.1-incubating Released");
        ASSERT_STREQ(result->emails[3]->subject,
                     "[jira] [Commented] (GORA-32) Map type with long values generates");
        ASSERT_EQ(result->size, 5);
    }

    void SetUp() override {
        params.recepient = (char*)malloc(30*sizeof(char));
        memmove(params.recepient, "gora-dev@incubator.apache.org", 30);
        char date_begin[] = "01 May 2011 00:00:00";
        params.begin = emailfilter_parse_date(date_begin);
        char date_end[] = "30 May 2011 00:00:00";
        params.end = emailfilter_parse_date(date_end);
    }

    void TearDown() override{
        free(params.recepient);
        free(params.begin);
        free(params.end);
    }
};

TEST_F(test_fixture_for_filter, unit_test1) {
    for (size_t i = 0; i < 4; i++) {
        emailfilterResult* result = emailfilter_filter(path.c_str(), &params, i);
        check_results(result);
        for (size_t k = 0; k < result->size; k++) {
            free(result->emails[k]->date);
            free(result->emails[k]->subject);
            for (int l = 0; l < result->emails[k]->recepients.size; l++){
                free(result->emails[k]->recepients.emails[l]);
            }
            free(result->emails[k]->recepients.emails);
            free(result->emails[k]);
        }

        free(result->emails);
        free(result);
    }
}

