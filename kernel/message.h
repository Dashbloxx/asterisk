#pragma once

#include "common.h"

typedef struct AsteriskMessage
{
    uint32_t message_type;
    uint32_t parameter1;
    uint32_t parameter2;
    uint32_t parameter3;
} AsteriskMessage;

typedef struct Thread Thread;

void message_send(Thread* thread, AsteriskMessage* message);

uint32_t message_get_queue_count(Thread* thread);

//returns remaining message count
int32_t message_get_next(Thread* thread, AsteriskMessage* message);