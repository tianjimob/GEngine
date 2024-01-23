#include "core/misc/config_cache_ini.h"
#include "catch_amalgamated.hpp"
#include <fstream>

TEST_CASE("get config from init", "[ConfigCacheIni]") {
  std::ofstream of{"tmp/tests/test_game.ini"};
  of << "[Window]" << std::endl;
  of << "width = 1920" << std::endl;
  of << "height = 1080" << std::endl;
  of.close();

  GEngine::ConfigCacheIni config;

  auto getWindowSettingInt = [&config](const std::string &key,
                                       int defaultValue) -> int32_t {
    int res = defaultValue;
    config.getInt("Window", key, "D:/code/GEngine/tmp/tests/test_game.ini", res);
    return res;
  };

  REQUIRE(getWindowSettingInt("width", 1280) == 1920);
  REQUIRE(getWindowSettingInt("height", 768) == 1080);
  
}