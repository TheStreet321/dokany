#include "DokanMemoryFS.h"
#include <spdlog/spdlog.h>

int main() {

  spdlog::set_level(spdlog::level::off);

  try {
    auto dokanMemoryFs = std::make_shared<DokanMemoryFS>();
    dokanMemoryFs->Run();
  } catch (const std::exception& ex) {
	spdlog::error("DokanMemoryFS failure: {}", ex.what());
  }
}