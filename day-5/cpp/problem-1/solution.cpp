// "Copyright 2023 anyone who wants this trash"
//
// KEY MAP INFO
// source-to-destination map:
// destination range start | source range start | range
//
// EXAMPLE:
// seed-to-soil map:
// 50 98 2
// 52 50 48
//
// line 1: soils [50-51] (range 2) map to seeds [98-99], therefore soil 50
// corresponds to seed 98, soil 51 to seed 99. line 2: soils [52-99] (range 48)
// map to seeds [50-97], therefore soil 52 corresponds to seed 50 ... soil 99 to
// seed 97
//

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
