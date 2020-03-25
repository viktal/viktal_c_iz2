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

    FILE *file = fopen(tw->path, "r");
    fseek(file, tw->start_index, SEEK_SET);
    stream_buffer_state *stream_buffer = create_stream_buffer();

    while (!feof(file)) {
        skip_to_new_line(file, stream_buffer);
        if (strncmp(stream_buffer->buffer,"From ", 5) == 0) {
            if (tw->real_num_mes >= tw->reserved_num_mes) {
                tw->reserved_num_mes = tw->reserved_num_mes == 0 ? 1 : 2 * (tw->reserved_num_mes);
                tw->list_mes = (emailfilterMessage**)realloc(tw->list_mes, tw->reserved_num_mes * sizeof(emailfilterMessage *));
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
        }

        if (ftell(file) >= tw->end_index)
            return NULL;
    }
    return NULL;
}

int compare(const void* message1, const void* message2)   // функция сравнения элементов массива
{
    emailfilterMessage* mes1 = *(emailfilterMessage**)(message1);
    emailfilterMessage* mes2 = *(emailfilterMessage**)(message2);
    return compare_date(mes1->date, mes2->date);
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
        tw.reserved_num_mes = 0;
        tw.real_num_mes = 0;
        tw.list_mes = NULL;
        tw.params = params;
        thread(&tw);
        result->size = tw.real_num_mes;
        result->emails = tw.list_mes;
    }
    else {
        int bytes_per_thread = (int)finfo.st_size / num_threads;
        //выделяем память под массив идентификаторов потоков
        pthread_t* threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
        //сколько потоков - столько и структур с данными
        emailfilterThreadWorkload* thread_data =
                (emailfilterThreadWorkload*) malloc(num_threads * sizeof(emailfilterThreadWorkload));

        //инициализируем структуры потоков
        for(int i = 0; i < num_threads; i++) {
            thread_data[i].path = path;
            thread_data[i].start_index = i * bytes_per_thread;
            thread_data[i].end_index = (i + 1) * bytes_per_thread;
            thread_data[i].reserved_num_mes = 0;
            thread_data[i].real_num_mes = 0;
            thread_data[i].list_mes = NULL;
            thread_data[i].params = params;

            //запускаем поток
            pthread_create(&(threads[i]), NULL, thread, &thread_data[i]);
        }
        //ожидаем выполнение всех потоков и склеиваем результат
        size_t total_messages = 0;
        for(int i = 0; i < num_threads; i++){
            pthread_join(threads[i], NULL);
            total_messages += thread_data[i].real_num_mes;
        }

        result->size = total_messages;
        result->emails = (emailfilterMessage**)malloc(sizeof(emailfilterMessage*) * total_messages);

        size_t next_ind = 0;
        for(int i = 0; i < num_threads; i++)
            if (thread_data[i].list_mes != NULL) {
            memmove(&(result->emails[next_ind]), thread_data[i].list_mes, sizeof(emailfilterMessage*) * thread_data[i].real_num_mes);
            next_ind += thread_data[i].real_num_mes;
        }

        // освобождаем память
        for (size_t i = 0; i < num_threads; i++)
            free(thread_data[i].list_mes);
        free(threads);
        free(thread_data);
    }
    // сортируем сообщения по дате
    qsort(result->emails, result->size, sizeof(emailfilterMessage*), compare);
    return result;
}
