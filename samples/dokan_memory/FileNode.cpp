#include "filenode.h"
#include <spdlog/spdlog.h>

namespace memfs {
filenode::filenode(std::wstring filename, bool is_directory, DWORD file_attr,
                   PDOKAN_IO_SECURITY_CONTEXT security_context)
    : _fileName(filename), is_directory(is_directory), attributes(file_attr) {
  // No lock need, FileNode is still not in a directory
  times.Reset();

  if (security_context && security_context->AccessState.SecurityDescriptor) {
    spdlog::info(L"{} : Attach SecurityDescriptor", filename);
    security.SetDescriptor(security_context->AccessState.SecurityDescriptor);
  }
}

DWORD filenode::read(LPVOID buffer, DWORD bufferlength, LONGLONG offset) {
  std::lock_guard<std::mutex> lock(_data_mutex);
  if (static_cast<size_t>(offset + bufferlength) > _data.size())
    bufferlength = static_cast<DWORD>(_data.size() - offset);
  if (bufferlength)
    memcpy(buffer, &_data[static_cast<size_t>(offset)], bufferlength);
  spdlog::info(L"Read {} : BufferLength {} Offset {}", get_filename(),
               bufferlength, offset);
  return bufferlength;
}

DWORD filenode::write(LPCVOID buffer, DWORD number_of_bytes_to_write,
                      LONGLONG offset) {
  if (!number_of_bytes_to_write) return 0;

  std::lock_guard<std::mutex> lock(_data_mutex);
  if (static_cast<size_t>(offset + number_of_bytes_to_write) > _data.size())
    _data.resize(static_cast<size_t>(offset + number_of_bytes_to_write));

  spdlog::info(L"Write {} : NumberOfBytesToWrite {} Offset {}", get_filename(),
               number_of_bytes_to_write, offset);
  memcpy(&_data[static_cast<size_t>(offset)], buffer, number_of_bytes_to_write);
  return number_of_bytes_to_write;
}

const LONGLONG filenode::get_filesize() {
  std::lock_guard<std::mutex> lock(_data_mutex);
  return static_cast<LONGLONG>(_data.size());
}

void filenode::set_endoffile(const LONGLONG& byte_offset) {
  std::lock_guard<std::mutex> lock(_data_mutex);
  _data.resize(static_cast<size_t>(byte_offset));
}

const std::wstring filenode::get_filename() {
  std::lock_guard<std::mutex> lock(_fileName_mutex);
  return _fileName;
}

void filenode::set_filename(const std::wstring& f) {
  std::lock_guard<std::mutex> lock(_fileName_mutex);
  _fileName = f;
}

void filenode::add_stream(std::shared_ptr<filenode> stream) {
  std::lock_guard<std::mutex> lock(_data_mutex);
  _streams[stream->get_filename()] = stream;
}

void filenode::remove_stream(std::shared_ptr<filenode> stream) {
  std::lock_guard<std::mutex> lock(_data_mutex);
  _streams.erase(stream->get_filename());
}

std::unordered_map<std::wstring, std::shared_ptr<filenode> >
filenode::get_streams() {
  std::lock_guard<std::mutex> lock(_data_mutex);
  return _streams;
}
}  // namespace memfs