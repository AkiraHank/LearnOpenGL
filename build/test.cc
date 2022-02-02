#include <cstdio>
#include <functional>
class test {
public:
  test(){}

  void func1() {
    printf("calling func1!\n");
  }

  void func2() {
    printf("calling func2!\n");
  }
};

int main() {
  test t;
  std::function<void(void)> pf = std::bind(&test::func1, t);
  pf();
  return 0;
}