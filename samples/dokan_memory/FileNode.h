#pragma once

#include <dokan/dokan.h>
#include <dokan/fileinfo.h>

#include "MemoryFSHelper.h"

#include <winbase.h>
#include <atomic>
#include <filesystem>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>

struct SecurityInformations : std::mutex {
  PSECURITY_DESCRIPTOR Descriptor = nullptr;
  DWORD DescriptorSize = 0;

  ~SecurityInformations() {
    if (Descriptor) delete[] Descriptor;
  }

  void SetDescriptor(PSECURITY_DESCRIPTOR SecurityDescriptor) {
    if (Descriptor) delete[] Descriptor;
    if (!SecurityDescriptor) return;
    DescriptorSize = GetSecurityDescriptorLength(SecurityDescriptor);
    Descriptor = new byte[DescriptorSize];
    memcpy(Descriptor, SecurityDescriptor, DescriptorSize);
  }
};

struct FileTimes {
  void Reset() {
    FILETIME t;
    GetSystemTimeAsFileTime(&t);
    LastAccess = LastWrite = Creation =
        MemoryFSHelper::DDwLowHighToLlong(t.dwLowDateTime, t.dwHighDateTime);
  }

  static bool isEmpty(CONST FILETIME* fileTime) {
    return fileTime->dwHighDateTime == 0 && fileTime->dwLowDateTime == 0;
  }

  std::atomic<LONGLONG> Creation;
  std::atomic<LONGLONG> LastAccess;
  std::atomic<LONGLONG> LastWrite;
};

class FileNode {
 public:
  FileNode(std::wstring fileName, bool isDirectory, DWORD fileAttr,
           PDOKAN_IO_SECURITY_CONTEXT SecurityContext);

  FileNode(const FileNode& f) = delete;

  DWORD Read(LPVOID Buffer, DWORD BufferLength, LONGLONG Offset);
  DWORD Write(LPCVOID Buffer, DWORD NumberOfBytesToWrite, LONGLONG Offset);

  const LONGLONG getFileSize();
  void setEndOfFile(const LONGLONG& byteOffset);

  // FileName change during move
  const std::wstring getFileName();
  void setFileName(const std::wstring& f);

  void AddStream(std::shared_ptr<FileNode> stream);
  void RemoveStream(std::shared_ptr<FileNode> stream);
  std::unordered_map<std::wstring, std::shared_ptr<FileNode> > GetStreams();

  // No lock needed above
  std::atomic<bool> IsDirectory = false;
  std::shared_ptr<FileNode> MainStream;
  std::atomic<DWORD> Attributes = 0;
  LONGLONG FileIndex = 0;

  FileTimes Times;
  SecurityInformations Security;

 private:
  FileNode() = default;

  std::mutex _data_mutex;
  // _data_mutex need to be aquired
  std::vector<uint8_t> _data;

  std::mutex _fileName_mutex;
  std::wstring _fileName;
  std::unordered_map<std::wstring, std::shared_ptr<FileNode> > _streams;
};