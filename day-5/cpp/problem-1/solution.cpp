// "Copyright 2023 anyone who wants this trash"

#include <string>
#include <vector>
std::vector<std::string> read_line(const std::string &file_path);

int main(int argc, char **argv) { std::cout << "Hello, world!"; }

std::vector<std::string> read_file(const std::string &file_path) {
  std::vector<std::string> v;
  std::ifstream f(file_path.c_str());
  std::string buffer;
  if (f.good()) {
    while (std::getline(f, buffer)) {
      v.push_back(buffer);
    }
  }
  return v;
}
