/*
  Dokan : user-mode file system library for Windows

  Copyright (C) 2019 Adrien J. <liryna.stark@gmail.com>
  Copyright (C) 2020 Google, Inc.

  http://dokan-dev.github.io

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef MEMFS_HELPER_H_
#define MEMFS_HELPER_H_

#include <windows.h>

namespace memfs {
// Memfs helpers
class memfs_helper {
 public:
  static inline LONGLONG FileTimeToLlong(const FILETIME& f) {
    return DDwLowHighToLlong(f.dwLowDateTime, f.dwHighDateTime);
  }

  static inline void LlongToFileTime(LONGLONG v, FILETIME& filetime) {
    LlongToDwLowHigh(v, filetime.dwLowDateTime, filetime.dwHighDateTime);
  }

  static inline LONGLONG DDwLowHighToLlong(const DWORD& low,
                                           const DWORD& high) {
    return static_cast<LONGLONG>(high) << 32 | low;
  }

  static inline void LlongToDwLowHigh(const LONGLONG& v, DWORD& low,
                                      DWORD& hight) {
    hight = v >> 32;
    low = static_cast<DWORD>(v);
  }
};
}  // namespace memfs

#endif MEMFS_HELPER_H_