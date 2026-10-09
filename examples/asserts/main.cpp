#include <massert.hpp>

int main() {
  int buffer_size = -5;

  massert(buffer_size > 0, "Buffer dimension must be positive",
          VAR(buffer_size));

  return 0;
}
