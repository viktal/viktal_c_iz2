#pragma once
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum emailfilterMonth {
    jan = 1, feb, mar, apr, may, jun,
    jul, aug, sep, oct, nov, dec
}emailfilterMonth;

typedef struct emailfilterDate {
    int year;
    emailfilterMonth month;
    int day;
    int hour;
    int minute;
    int second;
}emailfilterDate;

emailfilterDate* emailfilter_parse_date(char* date);
bool emailfilter_is_date(int d, emailfilterMonth mon, int y, int h, int min, int s);
bool emailfilter_is_leapyear(int y);
int emailfilter_compare_date(emailfilterDate* date1, emailfilterDate* date2);
emailfilterMonth emailfilter_parse_month(char* month);
void emailfilter_print_date(FILE* file, emailfilterDate* date);
