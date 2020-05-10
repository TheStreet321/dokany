#pragma once

#include <dokan/dokan.h>

namespace memfs {
// Memfs Dokan API implementation.
extern DOKAN_OPERATIONS memfs_operations;

// Helper getting the memfs filenodes context at each Dokan API call.
#define GET_FS_INSTANCE \
  reinterpret_cast<fs_filenodes*>(dokanfileinfo->DokanOptions->GlobalContext)
}  // namespace memfs