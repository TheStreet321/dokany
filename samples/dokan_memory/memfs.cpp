#include "memfs.h"

namespace memfs {
void memfs::run() {
  fs_filenodes = std::make_unique<::memfs::fs_filenodes>();

  DOKAN_OPTIONS dokan_options;
  ZeroMemory(&dokan_options, sizeof(DOKAN_OPTIONS));
  dokan_options.Version = DOKAN_VERSION;
  dokan_options.ThreadCount = 1;
  dokan_options.Options =  // DOKAN_OPTION_STDERR | DOKAN_OPTION_DEBUG |
      DOKAN_OPTION_MOUNT_MANAGER | DOKAN_OPTION_ALT_STREAM;
  dokan_options.MountPoint = L"M";
  dokan_options.GlobalContext = reinterpret_cast<ULONG64>(fs_filenodes.get());

  NTSTATUS status = DokanMain(&dokan_options, &memfs_operations);
  switch (status) {
    case DOKAN_SUCCESS:
      break;
    case DOKAN_ERROR:
      throw std::runtime_error("Error");
      break;
    case DOKAN_DRIVE_LETTER_ERROR:
      throw std::runtime_error("Bad Drive letter");
      break;
    case DOKAN_DRIVER_INSTALL_ERROR:
      throw std::runtime_error("Can't install driver");
      break;
    case DOKAN_START_ERROR:
      throw std::runtime_error("Driver something wrong");
      break;
    case DOKAN_MOUNT_ERROR:
      throw std::runtime_error("Can't assign a drive letter");
      break;
    case DOKAN_MOUNT_POINT_ERROR:
      throw std::runtime_error("Mount point error");
      break;
    case DOKAN_VERSION_ERROR:
      throw std::runtime_error("Version error");
      break;
    default:
      throw std::runtime_error("Unknown error");  // add error status
      break;
  }
}
}  // namespace memfs