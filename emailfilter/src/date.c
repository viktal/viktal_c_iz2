#include "emailfilter/date.h"

bool is_date(int d, emailfilterMonth mon, int y, int h, int min, int s)
{
    if (d<=0) return false;
    if (mon < jan || dec < mon) return false;

    int days_in_month = 31; // большинство месяцев состоит из 31 дня
    switch (mon) {
        case feb: // продолжительность февраля варьируется
            days_in_month = (is_leapyear(y))?29:28;
            break;
        case apr:
        case jun:
        case sep:
        case nov:
            days_in_month = 30;
            // остальные месяцы состоят из 30 дней
            break;
    }
    if (days_in_month<d) return false;
    if (h < 0 || h >= 24) //количество часов
        return false;
    if (min < 0 || min >= 60) //количество минут
        return false;
    if (s < 0 || s >= 60) //количество секунд
        return false;
    return true;
}

bool is_leapyear(int y)
{
    return (y%4==0 && y%100!=0) || y%400==0;
}


/* функция сравнения, которая возвращает
 * отрицательное целое значение, если первый аргумент меньше чем второй,
 * положительное целое значение, если первый аргумент больше чем второй
 * и ноль если они равны. */
int compare_date(emailfilterDate* date1, emailfilterDate* date2)
{
    int a = date1->year - date2->year;
    int b = date1->month - date2->month;
    int c = date1->day - date2->day;

    if (a != 0) return a;
    else if (b != 0) return b;
    else return c;
//    return (date1->year < date2->year) ||
//           (date1->year == date2->year && ( (date1->month < date2->month) ||
//           (date1->month == date2->month && date1->day < date2->day) ) );
}

void print_date(emailfilterDate* date)
{
    printf("%i.%i.%i %i:%i:%i \n", date->day, date->month, date->year, date->hour, date->minute, date->second);
}

emailfilterMonth set_month(char* m)
{
    for (int i = 0; i<3; i++){
        m[i] = tolower(m[i]);
    }
    emailfilterMonth month = 0;

    if (strcmp(m, "jan") == 0) month = 1;
    if (strcmp(m, "feb") == 0) month = 2;
    if (strcmp(m, "mar") == 0) month = 3;
    if (strcmp(m, "apr") == 0) month = 4;
    if (strcmp(m, "may") == 0) month = 5;
    if (strcmp(m, "jun") == 0) month = 6;
    if (strcmp(m, "jul") == 0) month = 7;
    if (strcmp(m, "aug") == 0) month = 8;
    if (strcmp(m, "sep") == 0) month = 9;
    if (strcmp(m, "oct") == 0) month = 10;
    if (strcmp(m, "nov") == 0) month = 11;
    if (strcmp(m, "dec") == 0) month = 12;

    if (!month) {
        printf("%s is not month \n", m);
        exit(EXIT_FAILURE);
    }
    else return month;
}

emailfilterDate* read_date(char* read_from)
{
    emailfilterDate *date = (emailfilterDate *) malloc(sizeof(emailfilterDate));
    int d, y;
    int h, min, s;
    char* m = (char*) malloc(4);
    sscanf(read_from, "%d %3s %d %d:%d:%d", &d, m, &y, &h, &min, &s); //DD.MM.YYYY

    emailfilterMonth mon = set_month(m);

    if(is_date(d, mon, y, h, min, s)){
        date->day = d;
        date->month = mon;
        date->year = y;
        date->hour = h;
        date->minute = min;
        date->second = s;
        return date;
    }
    else {
        printf("%s is not date \n", read_from);
        exit(EXIT_FAILURE);
    }
}