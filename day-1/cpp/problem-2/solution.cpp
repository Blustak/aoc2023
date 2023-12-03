#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

bool f_exists(std::string &file_path);
std::vector<std::string> read_file_to_vec(std::string &file_path);
unsigned int digit_from_first_and_last(std::string line);

int main(int argc, char **argv) {
  // input sanitation
  if (argc != 2) {
    std::cout << "Wrong number of inputs. You gave " << argc - 1
              << ", this program accepts 1 (file_path).\n";
    return 0;
  } else {
    std::string file_path = argv[1];
    if (!(f_exists(file_path))) {
      std::cout << "Sorry, but file " << file_path << " could not be read.\n";
      return 0;
    }

    // Running logical functions
    std::vector<std::string> calibrations = read_file_to_vec(file_path);
    unsigned int sum = 0;
    std::cout << "The first line is " << *calibrations.begin() << "\n";
    for (std::vector<std::string>::iterator iter = calibrations.begin();
         iter < calibrations.end(); iter++) {
      sum += digit_from_first_and_last(*iter);
    }
    std::cout << "All done! I summed to " << sum
              << ". Thanks for solving! <3\n";
  }
  return 0;
}

bool f_exists(std::string &file_path) {
  std::ifstream f(file_path.c_str());
  return f.good();
}

std::vector<std::string> read_file_to_vec(std::string &file_path) {
  std::ifstream TextFile(file_path.c_str());
  std::string buffer;
  std::vector<std::string> v;

  while (std::getline(TextFile, buffer)) {
    v.push_back(buffer);
  }
  return v;
}

unsigned int digit_from_first_and_last(std::string line) {
  std::map<std::string, int> translations = {
      {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
      {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};
  std::map<int, int> map;
  // get digits
  for (int i = 0; i < line.size(); i++) {
    if (isdigit(line[i])) {
      map.insert(std::make_pair(i, line[i] - '0'));
    }
  }
  // get longways digits
  for (auto e : translations) {
    std::size_t pos = line.find(e.first);
    while (pos != std::string::npos) {
      map.insert(std::make_pair(pos, e.second));
      pos = line.find(e.first, pos + e.first.size());
    }
  }
  return (map.begin()->second * 10) + map.rbegin()->second;
}
