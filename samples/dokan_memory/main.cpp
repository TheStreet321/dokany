#include "memfs.h"
#include <spdlog/spdlog.h>

int main() {

  spdlog::set_level(spdlog::level::off);

  try {
    auto dokan_memfs = std::make_shared<memfs::memfs>();
    dokan_memfs->run();
  } catch (const std::exception& ex) {
	spdlog::error("dokan_memfs failure: {}", ex.what());
  }
}