#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "emailfilter/filter.h"
#include "filter.h"

emailfilterResult* emailfilter_filter(const char* path, emailfilterParams* params, uint16_t num_threads) {
    struct stat finfo;
    stat(path, &finfo);
    emailfilterResult* result = (emailfilterResult*) malloc(sizeof(emailfilterResult));
    if (result == NULL)
        exit(EXIT_FAILURE);

    if (num_threads == 0) {
        fprintf(stdout, "emailfilter_filter: number of threads replaced %d->1. "
                               "Only sequential implementation available", num_threads);
        num_threads = 1;
    }

    emailfilterThreadWorkload tw;
    tw.path = path;
    tw.start_index = 0;
    tw.end_index = (size_t)finfo.st_size;
    tw.reserved_num_mes = 0;
    tw.real_num_mes = 0;
    tw.list_mes = NULL;
    tw.params = params;

    thread(&tw);

    result->size = tw.real_num_mes;
    result->emails = tw.list_mes;



    // сортируем сообщения по дате
    qsort(result->emails, result->size, sizeof(emailfilterMessage*), compare);
    return result;
}
