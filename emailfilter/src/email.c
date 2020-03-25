#include "emailfilter/email.h"

void emailfilter_parse_message(emailfilterBuffer* stream_buffer, emailfilterMessage* mes, FILE* file) {
    char* line = NULL;
    if (strncmp(stream_buffer->buffer, "Date:", 5) == 0) {
        emailfilter_read_to_newline(file, stream_buffer, &line);
        // "Date: Tue, 09 Jan 2018 13:57:33 +0100\n"
        // offset = 11 чтобы пропустить Date и день недели
        int offset = 11;
        line[offset + 20] = '\0';  // упускаем постфикс
        mes->date = emailfilter_parse_date(line + offset);
    } else if (strncmp(stream_buffer->buffer, "Subject:", 8) == 0) {
        emailfilter_read_to_newline(file, stream_buffer, &(line));
        // offset = 9 чтобы пропустить Subject:\space
        size_t size = strlen(line) - 9;
        mes->subject = (char*)malloc(size + 1);
        if (mes->subject == NULL)
            exit(EXIT_FAILURE);
        memmove(mes->subject, line + 9, size);
        mes->subject[size] = '\0';
    } else if (strncmp(stream_buffer->buffer, "To:", 3) == 0) {
        emailfilter_read_to_newline(file, stream_buffer, &(line));
        line[strlen(line)] = '\0';  // экранируем конец строки
        emailfilter_parse_recepients(line + 4, &(mes->recepients));
    }

    if (line != NULL)
        free(line);
}

void emailfilter_print_mes(FILE* file, emailfilterMessage** messages, int size) {
    for (int i = 0; i < size; i++) {
        fprintf(file, "Sub: %s\nTo:", messages[i]->subject);
        for (int j = 0; j < messages[i]->recepients.size; j++) {
            fprintf(file, "%s ", messages[i]->recepients.emails[j]);
        }
        printf("\n");
        emailfilter_print_date(file, messages[i]->date);
    }
    fprintf(file,"\n%i messages in total\n", size);
}

bool emailfilter_is_message_end(emailfilterMessage* mes) {
    if (mes->recepients.emails == NULL) return false;
    if (mes->date == NULL) return false;
    if (mes->subject == NULL) return false;
    return true;
}

void emailfilter_parse_recepients(char* line, emailfilterRecepients* recepients) {
    assert(recepients->emails == NULL);
    int reserved_size = 1;
    recepients->emails = (char **) malloc(reserved_size * sizeof(char *));
    if (recepients->emails == NULL)
        exit(EXIT_FAILURE);
    recepients->size = 0;

    char *copyfrom = NULL;
    size_t copysize = 0;

    char *emailblock = strtok_r(line, ", ", &line);
    while (emailblock != NULL) {
        recepients->size++;
        if (recepients->size > reserved_size) {
            reserved_size = 2 * reserved_size;
            recepients->emails = (char **) realloc(recepients->emails, reserved_size * sizeof(char *));
            if (recepients->emails == NULL)
                exit(EXIT_FAILURE);
        }
        char *email_begin = strchr(emailblock, '<');
        if (email_begin == NULL) {
            copyfrom = emailblock;
            copysize = strlen(emailblock);
        } else {
            char *email_end = strchr(email_begin, '>');
            copyfrom = email_begin + 1;
            copysize = email_end - email_begin;
        }
        recepients->emails[recepients->size - 1] = (char *) malloc((copysize + 1) * sizeof(char));
        if (recepients->emails[recepients->size - 1] == NULL)
            exit(EXIT_FAILURE);
        memmove(recepients->emails[recepients->size - 1], copyfrom, copysize);
        recepients->emails[recepients->size - 1][copysize] = '\0';
        emailblock = strtok_r(NULL, ", ", &line);
    }
}
