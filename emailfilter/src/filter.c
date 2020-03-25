#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "emailfilter/utils.h"
#include "emailfilter/date.h"
#include "emailfilter/filter.h"
#include "emailfilter/email.h"


typedef struct emailfilterThreadWorkload
{
    char* path;
    size_t start_index;
    size_t end_index;
    emailfilterMessage** list_mes;
    int reserved_num_mes;
    int real_num_mes;
    emailfilterParams* params;
} emailfilterThreadWorkload;


#define N 1 // кол-во потоков
#define K 2 // кол-во сообщений для потока

bool to_keep(emailfilterMessage* mes, emailfilterParams* params)
{
    if (compare_date(mes->date, params->begin) < 0)
        return false;
    else if (compare_date(mes->date, params->end) > 0)
        return false;
    else {
        for (int i = 0; i < mes->recepients.size; i++) {
            if (strcmp(mes->recepients.emails[i], params->recepient) == 0)
                return true;
        }
    }
    return false;
}

void *thread(void *arg) {
    emailfilterThreadWorkload *tw = (emailfilterThreadWorkload *) arg;

    tw->list_mes = (emailfilterMessage **) malloc((tw->reserved_num_mes) * sizeof(emailfilterMessage *));
    FILE *file = fopen(tw->path, "r");
    fseek(file, tw->start_index, SEEK_SET);
    stream_buffer_state *stream_buffer = create_stream_buffer();

    while (!feof(file)) {
        skip_to_new_line(file, stream_buffer);
        if (strncmp(stream_buffer->buffer,"From ", 5) == 0) {
            if (tw->real_num_mes >= tw->reserved_num_mes) {
                tw->reserved_num_mes = 2 * (tw->reserved_num_mes);
                tw->list_mes = (emailfilterMessage**)realloc(tw->list_mes, tw->reserved_num_mes * sizeof(tw->list_mes));
                if (tw->list_mes == NULL) {
                    printf("error in realloc for message %d", tw->real_num_mes);
                    exit(EXIT_FAILURE);
                }
            }
            tw->list_mes[tw->real_num_mes]= (emailfilterMessage *) malloc(sizeof(emailfilterMessage));
            tw->list_mes[tw->real_num_mes]->recepients.emails = NULL;
            tw->list_mes[tw->real_num_mes]->subject = NULL;
            tw->list_mes[tw->real_num_mes]->date = NULL;

            while(!message_end(tw->list_mes[tw->real_num_mes])){ // считать три элемента (subject, date, to)
                skip_to_new_line(file, stream_buffer);
                parse_message(stream_buffer, tw->list_mes[tw->real_num_mes], file);
            }
            if (to_keep(tw->list_mes[tw->real_num_mes], tw->params))
                tw->real_num_mes ++;
            long a = ftell(file);
            long b = tw->end_index;
            printf("%li %li \n", a, b);
        }

        if (ftell(file) >= tw->end_index)
        {
            printf("end of file\n");
            return NULL;
        }
    }
    return NULL;
}

emailfilterResult* emailfilter_filter(char* path, emailfilterParams* params, uint16_t num_threads) {
    struct stat finfo;
    stat(path, &finfo);
    emailfilterResult* result = (emailfilterResult*) malloc(sizeof(emailfilterResult));

    // sequential
    if (num_threads <= 1) {
        emailfilterThreadWorkload tw;
        tw.path = path;
        tw.start_index = 0;
        tw.end_index = (size_t)finfo.st_size;
        tw.reserved_num_mes = K;
        tw.real_num_mes = 0;
        tw.params = params;
        thread(&tw);
        result->size = tw.real_num_mes;
        result->emails = tw.list_mes;
    }
    else {
        int bytes_per_thread = (int)finfo.st_size / N;

        //Message** array_of_threads_mes = (Message**) malloc(N * sizeof(Message*));

        //выделяем память под массив идентификаторов потоков
        pthread_t* threads = (pthread_t*) malloc(N * sizeof(pthread_t));
        //сколько потоков - столько и структур с потоковых данных
        emailfilterThreadWorkload* thread_data =
                (emailfilterThreadWorkload*) malloc(N * sizeof(emailfilterThreadWorkload));

        //инициализируем структуры потоков
        for(int i = 0; i < N; i++){
            thread_data[i].path = path;
            thread_data[i].start_index = i * bytes_per_thread;
            thread_data[i].end_index = (i + 1) * bytes_per_thread;
            thread_data[i].reserved_num_mes = K;
            thread_data[i].real_num_mes = 0;
            thread_data[i].params = params;

            //запускаем поток
            pthread_create(&(threads[i]), NULL, thread, &thread_data[i]);
            printf("1 %i\n", thread_data[0].real_num_mes);
        }


        printf("1.1 %i\n", thread_data[0].real_num_mes);
        //ожидаем выполнение всех потоков
        for(int i = 0; i < N; i++){
            printf("2 %i\n", thread_data[0].real_num_mes);
            pthread_join(threads[i], NULL);
            printf("3 %i\n", thread_data[0].real_num_mes);
        }
        printf("4 %i\n", thread_data[0].real_num_mes);
    }
    //освобождаем память
    return result;
}
