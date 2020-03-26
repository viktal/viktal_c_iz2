#include <pthread.h>
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
        fprintf(stdout, "emailfilter_filter: number of threads replaced 0->1");
        num_threads = 1;
    }

    int bytes_per_thread = (int)finfo.st_size / num_threads;
    // выделяем память под массив идентификаторов потоков
    pthread_t* threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL)
        exit(EXIT_FAILURE);
    // сколько потоков - столько и структур с данными
    emailfilterThreadWorkload* thread_data =
            (emailfilterThreadWorkload*) malloc(num_threads * sizeof(emailfilterThreadWorkload));
    if (thread_data == NULL)
        exit(EXIT_FAILURE);

    // инициализируем структуры потоков
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].path = path;
        thread_data[i].start_index = i * bytes_per_thread;
        thread_data[i].end_index = (i + 1) * bytes_per_thread;
        thread_data[i].reserved_num_mes = 0;
        thread_data[i].real_num_mes = 0;
        thread_data[i].list_mes = NULL;
        thread_data[i].params = params;

        // запускаем поток
        pthread_create(&(threads[i]), NULL, thread, &thread_data[i]);
    }
    // ожидаем выполнение всех потоков и склеиваем результат
    size_t total_messages = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_messages += thread_data[i].real_num_mes;
    }

    result->size = total_messages;
    result->emails = (emailfilterMessage**)malloc(sizeof(emailfilterMessage*) * total_messages);
    if (result->emails == NULL)
        exit(EXIT_FAILURE);

    size_t next_ind = 0;
    for (int i = 0; i < num_threads; i++)
        if (thread_data[i].list_mes != NULL) {
            memmove(&(result->emails[next_ind]), thread_data[i].list_mes,
                    sizeof(emailfilterMessage*) * thread_data[i].real_num_mes);
            next_ind += thread_data[i].real_num_mes;
        }

    // освобождаем память
    for (size_t i = 0; i < num_threads; i++)
        free(thread_data[i].list_mes);
    free(threads);
    free(thread_data);

    // сортируем сообщения по дате
    qsort(result->emails, result->size, sizeof(emailfilterMessage*), compare);
    return result;
}
