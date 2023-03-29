#pragma once

#include "common.h"
#include "fs.h"

void sharedmemory_initialize();
filesystem_node* sharedmemory_create(const char* name);
BOOL sharedmemory_destroy_by_name(const char* name);
filesystem_node* sharedmemory_get_node(const char* name);
BOOL sharedmemory_unmap_if_exists(Process* process, uint32_t address);
void sharedmemory_unmap_for_process_all(Process* process);