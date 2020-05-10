#pragma once
#include <memory>
#include <mutex>

#include <iostream>
#include <set>
#include <unordered_map>
#include "filenode.h"

namespace memfs {
class fs_filenodes {
 public:
  fs_filenodes();

  NTSTATUS add(const std::shared_ptr<filenode>& filenode);

  std::shared_ptr<filenode> find(const std::wstring& filename);

  std::set<std::shared_ptr<filenode>> list_folder(const std::wstring& filename);

  void remove(const std::wstring& filename);
  void remove(const std::shared_ptr<filenode>& filenode);

  NTSTATUS move(std::wstring old_filename, std::wstring new_filename,
                BOOL replace_if_existing);

  static std::pair<std::wstring, std::wstring> get_stream_names(
      std::wstring real_filename);

 private:
  std::atomic<LONGLONG> _fs_fileindex_count = 1;

  // Mutex need to be aquired when using fileNodes / directoryPaths
  std::recursive_mutex _filesnodes_mutex;
  std::unordered_map<std::wstring, std::shared_ptr<filenode>> _filenodes;
  std::unordered_map<std::wstring, std::set<std::shared_ptr<filenode>>>
      _directoryPaths;
};
}  // namespace memfs
