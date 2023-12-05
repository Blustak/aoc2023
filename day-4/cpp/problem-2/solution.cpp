// "Copyright 2023 ya boi"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> read_file(const std::string &file_path);

int main(int argc, char **argv) {
  std::vector<std::string> lines = read_file(argv[1]);
  std::vector<std::string>::iterator iter;
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
    int score = 0;
    for (auto e : winning_nos) {
      if (std::find(my_nos.begin(), my_nos.end(), e) != std::end(my_nos)) {
        std::cout << "Winning no: " << e << "\n";
        ((score == 0) ? score = 1 : score *= 2);
      }
    }
    std::cout << "Score this round:" << score << "\n";
    cum_score += score;
  }
  std::cout << "Score i got was: " << cum_score << "\n";
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
