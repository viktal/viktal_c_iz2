#include "emailfilter/date.h"

bool emailfilter_is_date(int d, emailfilterMonth mon, int y, int h, int min, int s) {
    if (d <= 0) return false;
    if (mon < jan || dec < mon) return false;

    int days_in_month = 31;  // большинство месяцев состоит из 31 дня
    switch (mon) {
        case feb:  // продолжительность февраля варьируется
            days_in_month = (emailfilter_is_leapyear(y)) ? 29 : 28;
            break;
        case apr:
        case jun:
        case sep:
        case nov:
            days_in_month = 30;
            // остальные месяцы состоят из 30 дней
            break;
    }
    if (days_in_month < d) return false;
    if (h < 0 || h >= 24)  // количество часов
        return false;
    if (min < 0 || min >= 60)  // количество минут
        return false;
    if (s < 0 || s >= 60)  // количество секунд
        return false;
    return true;
}

bool emailfilter_is_leapyear(int y) {
    return (y%4 == 0 && y%100 != 0) || y%400 == 0;
}


/* функция сравнения, которая возвращает
 * отрицательное целое значение, если первый аргумент меньше чем второй,
 * положительное целое значение, если первый аргумент больше чем второй
 * и ноль если они равны. */
int emailfilter_compare_date(emailfilterDate* date1, emailfilterDate* date2) {
    int a = date1->year - date2->year;
    int b = date1->month - date2->month;
    int c = date1->day - date2->day;

    if (a != 0)
        return a;
    else if (b != 0)
        return b;
    else
        return c;
}

void emailfilter_print_date(FILE* file, emailfilterDate* date) {
    fprintf(file, "%i.%i.%i %i:%i:%i \n",
            date->day, date->month, date->year, date->hour, date->minute, date->second);
}

emailfilterMonth emailfilter_parse_month(char* month) {
    for (int i = 0; i < 3; i++) {
        month[i] = tolower(month[i]);
    }
    emailfilterMonth parsed_month = 0;

    if (strcmp(month, "jan") == 0) parsed_month = 1;
    if (strcmp(month, "feb") == 0) parsed_month = 2;
    if (strcmp(month, "mar") == 0) parsed_month = 3;
    if (strcmp(month, "apr") == 0) parsed_month = 4;
    if (strcmp(month, "may") == 0) parsed_month = 5;
    if (strcmp(month, "jun") == 0) parsed_month = 6;
    if (strcmp(month, "jul") == 0) parsed_month = 7;
    if (strcmp(month, "aug") == 0) parsed_month = 8;
    if (strcmp(month, "sep") == 0) parsed_month = 9;
    if (strcmp(month, "oct") == 0) parsed_month = 10;
    if (strcmp(month, "nov") == 0) parsed_month = 11;
    if (strcmp(month, "dec") == 0) parsed_month = 12;

    if (!parsed_month) {
        printf("%s is not month \n", month);
        exit(EXIT_FAILURE);
    } else {
        return parsed_month;
    }
}

emailfilterDate* emailfilter_parse_date(char* date) {
    emailfilterDate *parsed_date = (emailfilterDate *) malloc(sizeof(emailfilterDate));
    int d, y;
    int h, min, s;
    char* m = (char*) malloc(4);
    if (m == NULL)
        exit(EXIT_FAILURE);
    sscanf(date, "%d %3s %d %d:%d:%d", &d, m, &y, &h, &min, &s);  // DD.MM.YYYY

    emailfilterMonth mon = emailfilter_parse_month(m);
    free(m);

    if (emailfilter_is_date(d, mon, y, h, min, s)) {
        parsed_date->day = d;
        parsed_date->month = mon;
        parsed_date->year = y;
        parsed_date->hour = h;
        parsed_date->minute = min;
        parsed_date->second = s;
        return parsed_date;
    } else {
        printf("%s is not date \n", date);
        exit(EXIT_FAILURE);
    }
}
