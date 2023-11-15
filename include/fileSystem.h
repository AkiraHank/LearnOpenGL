#include <initializer_list>
class EditorFoundation {
 private:
  EditorFoundation() = default;

 public:
  static EditorFoundation* instance() {
    static EditorFoundation* instance;
    if (instance == nullptr) {
      instance = new EditorFoundation();
    }
    return instance;
  }

  std::string getResourceDir();
  std::string join(std::initializer_list<std::string> pathes);
};