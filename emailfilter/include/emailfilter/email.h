#pragma once

#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "emailfilter/utils.h"
#include "emailfilter/date.h"

typedef struct emailfilterRecepients {
    char** emails;
    int size;
}emailfilterRecepients;

typedef struct emailfilterMessage {
    emailfilterDate* date;
    char* subject;
    emailfilterRecepients recepients;
}emailfilterMessage;

void emailfilter_print_mes(FILE* file, emailfilterMessage** messages, int size);
bool emailfilter_is_message_end(emailfilterMessage* mes);
void emailfilter_parse_recepients(char* line, emailfilterRecepients* recepients);
void emailfilter_parse_message(emailfilterBuffer* stream_buffer, emailfilterMessage* mes, FILE* file);
