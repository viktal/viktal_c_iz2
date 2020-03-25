#pragma once

#include "emailfilter/date.h"
#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>

typedef struct emailfilterRecepients {
    char** emails;
    int size;
}emailfilterRecepients;

typedef struct emailfilterMessage{
    emailfilterDate* date;
    char* subject;
    emailfilterRecepients recepients;
}emailfilterMessage;

void print_mes(emailfilterMessage** messages, int size);
bool message_end(emailfilterMessage* mes);
void parse_recepients(char* line, emailfilterRecepients* recepients);
void parse_message(stream_buffer_state* stream_buffer, emailfilterMessage* mes, FILE* file);
