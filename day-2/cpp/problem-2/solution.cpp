// "Copyright 2023 Blustak"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum Color { RED, GREEN, BLUE };

std::vector<std::string> read_lines(const std::string &file_path);
int strip_to_number(std::string s);

int main(int argc, char **argv) {
  std::vector<std::string> lines = read_lines(argv[1]);
  std::string game_delimiter = ":";
  std::string round_delimiter = ";";
  std::string turn_delimiter = ",";
  int power_sum = 0;

  for (std::string line : lines) {
    int smallest_red = 0;
    int smallest_green = 0;
    int smallest_blue = 0;

    std::size_t delimiter_pos = line.find(game_delimiter);
    std::string round_token =
        line.substr(delimiter_pos + game_delimiter.length());
    // Split into rounds
    std::vector<std::string> turns;
    std::size_t needle_pos = 0;
    // remove whitespace for easier indexing
    round_token.erase(
        std::remove_if(round_token.begin(), round_token.end(), isspace),
        round_token.end());

    while (round_token.length() > 0) {
      needle_pos = round_token.find(round_delimiter);
      if (needle_pos == std::string::npos) {
        needle_pos = round_token.length();
      }
      std::string substr = round_token.substr(0, needle_pos);
      turns.push_back(substr);
      round_token.erase(0, needle_pos + round_delimiter.length());
    }
    for (std::string s : turns) {
      needle_pos = 0;
      while (s.length() > 0) {
        Color c;
        needle_pos = s.find(turn_delimiter);
        if (needle_pos == std::string::npos) {
          needle_pos = s.length();
        }
        std::string substr = s.substr(0, needle_pos);
        for (std::string::iterator i = substr.begin(); i < substr.end(); i++) {
          if (!(isdigit(*i))) {
            switch (*i) {
            case 'r':
              c = Color::RED;
              break;
            case 'b':
              c = Color::BLUE;
              break;
            case 'g':
              c = Color::GREEN;
              break;
            }
            break;
          }
        }
        int i = strip_to_number(substr);
        switch (c) {
        case Color::RED:
          if (i > smallest_red) {
            smallest_red = i;
          }
          break;
        case Color::BLUE:
          if (i > smallest_blue) {
            smallest_blue = i;
          }
          break;
        case Color::GREEN:
          if (i > smallest_green) {
            smallest_green = i;
          }
          break;
        }
        s.erase(0, needle_pos + turn_delimiter.length());
      }
    }
    power_sum += smallest_red * smallest_green * smallest_blue;
  }
  std::cout << "My sum came to " << power_sum << ".\n";
  return 0;
}

std::vector<std::string> read_lines(const std::string &file_path) {
  // Returns empty vector if it couldn't read.
  std::ifstream f(file_path.c_str());
  if (f.good()) {
    std::vector<std::string> v;
    std::string buffer;
    while (std::getline(f, buffer)) {
      v.push_back(buffer);
    }
    return v;
  } else {
    return std::vector<std::string>();
  }
}

int strip_to_number(std::string s) {
  // strips a string to its digits and returns them as an int. consumes the
  // string.
  s.erase(std::remove_if(s.begin(), s.end(),
                         [](unsigned char x) { return !(isdigit(x)); }),
          s.end());
  int i = std::stoi(s);
  s.clear(); // Explicitly consume.
  return i;
}
