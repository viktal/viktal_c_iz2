#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "emailfilter/filter.h"
#include "emailfilter/date.h"




int main(int argc, char** argv) {
    if (argc != 6)
    {
        printf("%s",
                "Usage: app <email database>emails.txt <recipient>monro@gmail.com "
                "<date begin>\"05 Jan 2019 13:18:49\" <date end>\"15 Jan 2020 00:00:00\" <num threads>12\n"
                "Example: app emails.db elena@gmail.com \"05 Jan 2019 13:18:49\" \"15 Jan 2020 00:00:00\" 1\n"
                "send threads -1 to use maximum number of threads available on the system\n"
        );
        return EXIT_SUCCESS;
    }
    char* path = argv[1];
    emailfilterParams params;
    params.recepient = argv[2];
    params.begin = read_date(argv[3]);
    params.end = read_date(argv[4]);
    int threads = 0;
    if (strcmp(argv[5], "-1") == 0)
        threads = get_nprocs();
    else
        sscanf(argv[5], "%d", &threads);
    emailfilterResult* result = emailfilter_filter(path, &params, threads);
    print_mes(result->emails, result->size);

    for (size_t i = 0; i < result->size; i++)
        free(result->emails[i]);
    free(result->emails);
    free(result);

    return EXIT_SUCCESS;
}
