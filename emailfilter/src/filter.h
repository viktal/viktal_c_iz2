#pragma once


typedef struct emailfilterThreadWorkload {
    const char* path;
    size_t start_index;
    size_t end_index;
    emailfilterMessage** list_mes;
    int reserved_num_mes;
    int real_num_mes;
    emailfilterParams* params;
} emailfilterThreadWorkload;


bool to_keep(emailfilterMessage* mes, emailfilterParams* params);
void *thread(void *arg);
int compare(const void* message1, const void* message2);

