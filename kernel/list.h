/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  BSD 2-Clause License
 *  Copyright (c) 2017, ozkl, Nexuss
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *  
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#pragma once

#include "common.h"

#define list_foreach(list_node, list) for (ListNode* list_node = list->head; NULL != list_node ; list_node = list_node->next)

typedef struct ListNode
{
    struct ListNode* previous;
    struct ListNode* next;
    void* data;
} ListNode;

typedef struct List
{
    struct ListNode* head;
    struct ListNode* tail;
} List;

List* list_create();
void list_clear(List* list);
void list_destroy(List* list);
List* list_create_clone(List* list);
BOOL list_is_empty(List* list);
void list_append(List* list, void* data);
void list_prepend(List* list, void* data);
ListNode* list_get_first_node(List* list);
ListNode* list_get_last_node(List* list);
ListNode* list_find_first_occurrence(List* list, void* data);
int list_find_first_occurrence_index(List* list, void* data);
int list_get_count(List* list);
void list_remove_node(List* list, ListNode* node);
void list_remove_first_node(List* list);
void list_remove_last_node(List* list);
void list_remove_first_occurrence(List* list, void* data);

typedef struct Stack
{
    List* list;
} Stack;

Stack* stack_create();
void stack_clear(Stack* stack);
void stack_destroy(Stack* stack);
BOOL stack_is_empty(Stack* stack);
void stack_push(Stack* stack, void* data);
void* stack_pop(Stack* stack);

typedef struct Queue
{
    List* list;
} Queue;

Queue* queue_create();
void queue_clear(Queue* queue);
void queue_destroy(Queue* queue);
BOOL queue_is_empty(Queue* queue);
void queue_enqueue(Queue* queue, void* data);
void* queue_dequeue(Queue* queue);