// "Copyright 2023 Me"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool f_exists(std::string &file_path);
std::vector<std::string> read_file_to_vec(std::string &file_path);
int digit_from_first_and_last(std::string line);
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
    int sum = 0;
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

int digit_from_first_and_last(std::string line) {
  bool found = false;
  char c[2];
  for (std::string::iterator iter = line.begin(); iter < line.end(); iter++) {
    if (*iter >= '0' && *iter <= '9') {
      c[0] = *iter;
      break;
    }
  }
  for (std::string::iterator iter = line.end(); iter >= line.begin(); iter--) {
    if (*iter >= '0' && *iter <= '9') {
      c[1] = *iter;
      break;
    }
  }
  std::string s(c);
  return std::stoi(s);
}
