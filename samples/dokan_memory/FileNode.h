#pragma once

#include <dokan/dokan.h>
#include <dokan/fileinfo.h>

#include "memfs_helper.h"

#include <winbase.h>
#include <atomic>
#include <filesystem>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>

namespace memfs {
struct security_informations : std::mutex {
  PSECURITY_DESCRIPTOR descriptor = nullptr;
  DWORD descriptor_size = 0;

  ~security_informations() {
    if (descriptor) delete[] descriptor;
  }

  void SetDescriptor(PSECURITY_DESCRIPTOR securitydescriptor) {
    if (descriptor) delete[] descriptor;
    if (!securitydescriptor) return;
    descriptor_size = GetSecurityDescriptorLength(securitydescriptor);
    descriptor = new byte[descriptor_size];
    memcpy(descriptor, securitydescriptor, descriptor_size);
  }
};

struct filetimes {
  void Reset() {
    FILETIME t;
    GetSystemTimeAsFileTime(&t);
    lastaccess = lastwrite = creation =
        memfs_helper::DDwLowHighToLlong(t.dwLowDateTime, t.dwHighDateTime);
  }

  static bool isEmpty(CONST FILETIME* filetime) {
    return filetime->dwHighDateTime == 0 && filetime->dwLowDateTime == 0;
  }

  std::atomic<LONGLONG> creation;
  std::atomic<LONGLONG> lastaccess;
  std::atomic<LONGLONG> lastwrite;
};

class filenode {
 public:
  filenode(std::wstring filename, bool is_directory, DWORD file_attr,
           PDOKAN_IO_SECURITY_CONTEXT security_context);

  filenode(const filenode& f) = delete;

  DWORD read(LPVOID buffer, DWORD bufferlength, LONGLONG offset);
  DWORD write(LPCVOID buffer, DWORD number_of_bytes_to_write, LONGLONG offset);

  const LONGLONG get_filesize();
  void set_endoffile(const LONGLONG& byte_offset);

  // FileName change during move
  const std::wstring get_filename();
  void set_filename(const std::wstring& filename);

  void add_stream(std::shared_ptr<filenode> stream);
  void remove_stream(std::shared_ptr<filenode> stream);
  std::unordered_map<std::wstring, std::shared_ptr<filenode> > get_streams();

  // No lock needed above
  std::atomic<bool> is_directory = false;
  std::shared_ptr<filenode> main_stream;
  std::atomic<DWORD> attributes = 0;
  LONGLONG fileindex = 0;

  filetimes times;
  security_informations security;

 private:
  filenode() = default;

  std::mutex _data_mutex;
  // _data_mutex need to be aquired
  std::vector<uint8_t> _data;
  std::unordered_map<std::wstring, std::shared_ptr<filenode> > _streams;

  std::mutex _fileName_mutex;
  // _fileName_mutex need to be aquired
  std::wstring _fileName;
};
}  // namespace memfs