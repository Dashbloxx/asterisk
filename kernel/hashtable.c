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
 
#include "hashtable.h"
#include "alloc.h"

typedef struct DataItem
{
   uint32_t data;
   uint32_t key;
   uint8_t used;
} DataItem;

typedef struct HashTable
{
   DataItem* items;
   uint32_t capacity;
} HashTable;

static uint32_t hash_code(HashTable* hashtable, uint32_t key)
{
   return key % hashtable->capacity;
}

HashTable* hashtable_create(uint32_t capacity)
{
    HashTable* hashtable = kmalloc(sizeof(HashTable));
    memset((uint8_t*)hashtable, 0, sizeof(HashTable));
    hashtable->capacity = capacity;
    hashtable->items = kmalloc(sizeof(DataItem) * capacity);

    return hashtable;
}

void hashtable_destroy(HashTable* hashtable)
{
    kfree(hashtable->items);
    kfree(hashtable);
}

DataItem* HashTable_search_internal(HashTable* hashtable, uint32_t key)
{
   //get the hash
   uint32_t hash_index = hash_code(hashtable, key);

   uint32_t counter = 0;
   while(counter < hashtable->capacity)
   {
      if(hashtable->items[hash_index].key == key)
      {
          if(hashtable->items[hash_index].used == TRUE)
          {
              return &(hashtable->items[hash_index]);
          }
      }

      //go to next cell
      ++hash_index;

      //wrap around the table
      hash_index %= hashtable->capacity;

      ++counter;
   }

   return NULL;
}

BOOL hashtable_search(HashTable* hashtable, uint32_t key, uint32_t* value)
{
    DataItem* existing = HashTable_search_internal(hashtable, key);

    if (existing)
    {
        *value = existing->data;

        return TRUE;
    }

    return FALSE;
}

BOOL hashtable_insert(HashTable* hashtable, uint32_t key, uint32_t data)
{
    DataItem* existing = HashTable_search_internal(hashtable, key);

    if (existing)
    {
        existing->data = data;

        return TRUE;
    }

    //get the hash
    uint32_t hash_index = hash_code(hashtable, key);

    uint32_t counter = 0;
    //move in array until an empty or deleted cell
    while(counter < hashtable->capacity)
    {
        if (hashtable->items[hash_index].used == FALSE)
        {
            hashtable->items[hash_index].key = key;
            hashtable->items[hash_index].data = data;
            hashtable->items[hash_index].used = TRUE;

            return TRUE;
        }


        //go to next cell
        ++hash_index;

        //wrap around the table
        hash_index %= hashtable->capacity;

        ++counter;
    }

    return FALSE;
}

BOOL hashtable_remove(HashTable* hashtable, uint32_t key)
{
    DataItem* existing = HashTable_search_internal(hashtable, key);

    if (existing)
    {
        existing->used = FALSE;

        return TRUE;
    }

    return FALSE;
}
