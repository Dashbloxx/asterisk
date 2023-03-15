#pragma once

#include "device.h"
#include "fs.h"
#include "common.h"

void devfs_initialize();
FileSystemNode* devfs_register_device(Device* device);