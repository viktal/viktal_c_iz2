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

emailfilterDate* read_date(char* read_from);
bool is_date(int d, emailfilterMonth mon, int y, int h, int min, int s);
bool is_leapyear(int y);
int compare_date(emailfilterDate* date1, emailfilterDate* date2);  // true, если первая дата меньше или равна второй
emailfilterMonth set_month(char* m);
void print_date(emailfilterDate* date);
