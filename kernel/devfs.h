#pragma once

#include "device.h"
#include "fs.h"
#include "common.h"

void devfs_initialize();
filesystem_node* devfs_register_device(Device* device);