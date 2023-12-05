// "Copyright 2023 ya boi"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> read_file(const std::string &file_path);

struct scratch_record {
  int instances;
  int wins;
};

int main(int argc, char **argv) {
  std::vector<std::string> lines = read_file(argv[1]);
  std::vector<std::string>::iterator iter;
  std::vector<scratch_record> record;
  unsigned int cum_score = 0;
  for (iter = lines.begin(); iter != lines.end(); iter++) {
    // First remove the card info
    std::string content;
    std::vector<int> winning_nos;
    std::vector<int> my_nos;
    std::size_t split_pos;
    content = iter->substr(iter->find(':') + 2);
    split_pos = content.find('|');
    std::istringstream iss;
    iss.str(content.substr(0, split_pos));
    std::string buffer;
    while (std::getline(iss, buffer, ' ')) {
      if (buffer.size() > 0) {

        winning_nos.push_back(std::stoi(buffer));
      }
    }
    iss.clear();
    iss.str(content.substr(split_pos + 1));
    while (std::getline(iss, buffer, ' ')) {
      if (buffer.size() > 0) {
        my_nos.push_back(std::stoi(buffer));
      }
    }
    int wins = 0;
    for (auto e : winning_nos) {
      if (std::find(my_nos.begin(), my_nos.end(), e) != std::end(my_nos)) {
        wins++;
      }
    }
    record.push_back(scratch_record{1, wins});
  }
  for (std::vector<scratch_record>::iterator iter = record.begin();
       iter != record.end(); iter++) {
    for (int i = 0; i != iter->instances; i++) {
      for (int j = 1; j <= iter->wins; j++) {
        std::next(iter, j)->instances++;
      }
    }
  }
  int cum_sum = 0;
  for (auto r : record) {
    cum_sum += r.instances;
  }
  std::cout << "The number of cards I got was " << cum_sum << ",\n";
  return 0;
}

// trim in place from start

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
