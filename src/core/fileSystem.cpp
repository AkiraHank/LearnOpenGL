#include <filesystem>

#include "fileSystem.h"

std::string EditorFoundation::getResourceDir() {
  return (std::filesystem::current_path() / "resources").string();
}

std::string EditorFoundation::join(std::initializer_list<std::string> pathes) {
  std::filesystem::path ret = "";
  for (auto pa = pathes.begin(); pa != pathes.end(); pa++) {
    ret /= std::filesystem::path(*pa);
  }
  return ret.string();
}