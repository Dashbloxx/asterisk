#include "message.h"
#include "process.h"
#include "fifobuffer.h"


void message_send(Thread* thread, AsteriskMessage* message)
{
    spinlock_lock(&(thread->message_queue_lock));

    fifobuffer_enqueue(thread->message_queue, (uint8_t*)message, sizeof(AsteriskMessage));

    spinlock_unlock(&(thread->message_queue_lock));
}

uint32_t message_get_queue_count(Thread* thread)
{
    int result = 0;

    spinlock_lock(&(thread->message_queue_lock));

    result = fifobuffer_get_size(thread->message_queue) / sizeof(AsteriskMessage);

    spinlock_unlock(&(thread->message_queue_lock));

    return result;
}

//returns remaining message count
int32_t message_get_next(Thread* thread, AsteriskMessage* message)
{
    uint32_t result = -1;

    spinlock_lock(&(thread->message_queue_lock));

    result = fifobuffer_get_size(thread->message_queue) / sizeof(AsteriskMessage);

    if (result > 0)
    {
        fifobuffer_dequeue(thread->message_queue, (uint8_t*)message, sizeof(AsteriskMessage));

        --result;
    }
    else
    {
        result = -1;
    }

    spinlock_unlock(&(thread->message_queue_lock));

    return result;
}
