#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "emailfilter/utils.h"
#include "emailfilter/date.h"
#include "emailfilter/filter.h"
#include "emailfilter/email.h"
#include "filter.h"

bool to_keep(emailfilterMessage* mes, emailfilterParams* params) {
    if (emailfilter_compare_date(mes->date, params->begin) < 0) {
        return false;
    } else if (emailfilter_compare_date(mes->date, params->end) > 0) {
        return false;
    } else {
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
    emailfilterBuffer *stream_buffer = emailfilter_create_default_buffer();

    while (!feof(file)) {
        emailfilter_skip_to_new_line(file, stream_buffer);
        if (strncmp(stream_buffer->buffer, "From ", 5) == 0) {
            if (tw->real_num_mes >= tw->reserved_num_mes) {
                tw->reserved_num_mes = tw->reserved_num_mes == 0 ? 1 : 2 * (tw->reserved_num_mes);
                tw->list_mes = (emailfilterMessage**)realloc(tw->list_mes,
                        tw->reserved_num_mes * sizeof(emailfilterMessage *));
                if (tw->list_mes == NULL) {
                    printf("error in realloc for message %d", tw->real_num_mes);
                    exit(EXIT_FAILURE);
                }
            }
            tw->list_mes[tw->real_num_mes]= (emailfilterMessage *) malloc(sizeof(emailfilterMessage));
            if (tw->list_mes[tw->real_num_mes] == NULL)
                exit(EXIT_FAILURE);
            tw->list_mes[tw->real_num_mes]->recepients.emails = NULL;
            tw->list_mes[tw->real_num_mes]->subject = NULL;
            tw->list_mes[tw->real_num_mes]->date = NULL;

            while (!emailfilter_is_message_end(tw->list_mes[tw->real_num_mes])) {
                // считать три элемента (subject, date, to)
                emailfilter_skip_to_new_line(file, stream_buffer);
                emailfilter_parse_message(stream_buffer, tw->list_mes[tw->real_num_mes], file);
            }
            if (to_keep(tw->list_mes[tw->real_num_mes], tw->params)) {
                tw->real_num_mes++;
            } else {
                for (int i = 0; i < tw->list_mes[tw->real_num_mes]->recepients.size; i++) {
                    free(tw->list_mes[tw->real_num_mes]->recepients.emails[i]);
                }
                free(tw->list_mes[tw->real_num_mes]->recepients.emails);
                free(tw->list_mes[tw->real_num_mes]->subject);
                free(tw->list_mes[tw->real_num_mes]->date);
                free(tw->list_mes[tw->real_num_mes]);
            }
        }

        if (ftell(file) >= tw->end_index) {
            fclose(file);
            free(stream_buffer->buffer);
            free(stream_buffer);
            return NULL;
        }
    }
    fclose(file);
    free(stream_buffer->buffer);
    free(stream_buffer);
    return NULL;
}

int compare(const void* message1, const void* message2) {  // функция сравнения элементов массива
    emailfilterMessage* mes1 = *(emailfilterMessage**)(message1);
    emailfilterMessage* mes2 = *(emailfilterMessage**)(message2);
    return emailfilter_compare_date(mes1->date, mes2->date);
}
