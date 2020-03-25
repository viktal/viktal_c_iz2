#include "gtest/gtest.h"

extern "C" {
#include "emailfilter/date.h"
}

TEST(date1, is_month_test)
{
    char m1[] = "Jan";
    ASSERT_EQ(set_month(m1), 1);
    ASSERT_NE(set_month(m1), 2);

    char m2[] = "Dec";
    ASSERT_EQ(set_month(m2), 12);

    char m3[] = "fdf";
    ASSERT_DEATH(set_month(m3), "");

}

TEST(date2, is_date_test)
{
    ASSERT_TRUE(is_date(1, (emailfilterMonth)1, 2020, 1, 1, 1));
    ASSERT_FALSE(is_date(33, (emailfilterMonth)1, 2020, 1, 1, 1));
    ASSERT_FALSE(is_date(1, (emailfilterMonth)13, 2020, 1, 1, 1));
    ASSERT_FALSE(is_date(1, (emailfilterMonth)1, 2020, 25, 1, 1));
    ASSERT_FALSE(is_date(1, (emailfilterMonth)1, 2020, 1, 70, 1));
    ASSERT_FALSE(is_date(1, (emailfilterMonth)1, 2020, 1, 1, 70));
    ASSERT_TRUE(is_date(29, (emailfilterMonth)2, 2020, 1, 1, 1));
    ASSERT_FALSE(is_date(29, (emailfilterMonth)2, 2019, 1, 1, 1));
}

TEST(date3, compare_date_test)
{
    emailfilterDate *date1 = (emailfilterDate *) malloc(sizeof(emailfilterDate));
    date1->day = 1;
    date1->month = (emailfilterMonth)1;
    date1->year = 2020;
    date1->hour = 1;
    date1->minute = 1;
    date1->second = 1;
    ASSERT_EQ(compare_date(date1, date1), 0);

    emailfilterDate *date2 = (emailfilterDate *) malloc(sizeof(emailfilterDate));
    date2->day = 2;
    date2->month = (emailfilterMonth)1;
    date2->year = 2020;
    date2->hour = 1;
    date2->minute = 1;
    date2->second = 1;
    ASSERT_EQ(compare_date(date1, date2), -1);
    ASSERT_EQ(compare_date(date2, date1), 1);
}
