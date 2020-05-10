#pragma once

#include <dokan/dokan.h>

namespace memfs {
extern DOKAN_OPERATIONS memfs_operations;

#define GET_FS_INSTANCE \
  reinterpret_cast<fs_filenodes*>(dokanfileinfo->DokanOptions->GlobalContext)
}  // namespace memfs