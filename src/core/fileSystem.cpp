#include "fileSystem.h"

#include <filesystem>

std::string EditorFoundation::getResourceDir() {
  return (std::filesystem::current_path() / "resources").string();
}

std::string EditorFoundation::join(std::initializer_list<std::string> _pathes) {
  std::filesystem::path ret = "";
  for (auto pa = _pathes.begin(); pa != _pathes.end(); pa++) {
    ret /= std::filesystem::path(*pa);
  }
  return ret.string();
}