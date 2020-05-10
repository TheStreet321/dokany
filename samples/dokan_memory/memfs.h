#pragma once
#include <dokan/dokan.h>
#include <dokan/fileinfo.h>

#include <winbase.h>
#include <iostream>
#include "filenodes.h"
#include "memfs_operations.h"

namespace memfs {
class memfs {
 public:
  void run();

  std::unique_ptr<fs_filenodes> fs_filenodes;
};
}  // namespace memfs