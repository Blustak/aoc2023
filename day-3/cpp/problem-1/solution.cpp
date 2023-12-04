// "Copyright 2023 Blustak"
// Very naive solution.I need to look up 2d pathfinding stuff.
#include <cctype>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct walkable_dimensions {
  bool left;
  bool right;
  bool up;
  bool down;
};

const char skip_char = '.';

std::vector<std::vector<char>> read_file(const std::string &file_name);

int main(int argc, char **argv) {
  std::vector<std::vector<char>> arena = read_file(argv[1]);
  std::vector<int> valid_parts;
  int x_left_bound = 0;
  int x_right_bound;
  int y_top_bound = 0;
  int y_bottom_bound = arena.size();

  // scan for digit
  for (int y = 0; y < y_bottom_bound; y++) {
    x_right_bound = arena[y].size() - 1; //-1 for end of line character perhaps?
    for (int x = 0; x < x_right_bound; x++) {
      if (isdigit(arena[y][x])) {
        std::string number;
        int x_offset = 0;
        std::vector<char> adj_symbols;
        bool above = y - 1 > 0;
        bool below = y + 1 < y_bottom_bound;
        // add the lefthand symbols
        if (x - 1 > 0) {
          adj_symbols.push_back(arena[y][x - 1]);
          if (above) {
            adj_symbols.push_back(arena[y - 1][x - 1]);
          }
          if (below) {
            adj_symbols.push_back(arena[y + 1][x - 1]);
          }
        }
        while (x + x_offset < x_right_bound &&
               isdigit(arena[y][x + x_offset])) {
          number += arena[y][x + x_offset];
          if (above) {
            adj_symbols.push_back(arena[y - 1][x + x_offset]);
          }
          if (below) {
            adj_symbols.push_back(arena[y + 1][x + x_offset]);
          }
          x_offset++;
        }
        // add the righthand symbols
        if (x + x_offset < x_right_bound) {
          adj_symbols.push_back(arena[y][x + x_offset]);
          if (above) {
            adj_symbols.push_back(arena[y - 1][x + x_offset]);
          }
          if (below) {
            adj_symbols.push_back(arena[y + 1][x + x_offset]);
          }
        }
        x = x + x_offset;

        // Now do the adding logic
        for (char c : adj_symbols) {
          if (c != skip_char) {
            std::cout << "Number: " << number << "\n";
            std::cout << "adjacent symbols: ";
            for (char c : adj_symbols) {
              std::cout << c << ", ";
            }
            std::cout << "\n";
            valid_parts.push_back(std::stoi(number));
            break;
          }
        }
      }
    }
  }

  unsigned int sum = 0;
  for (int i : valid_parts) {
    sum += i;
  }
  std::cout << "The final sum I got was... " << sum << "!\n";
}

std::vector<std::vector<char>> read_file(const std::string &file_name) {
  std::ifstream f(file_name);
  std::string buffer;
  std::vector<std::vector<char>> v;
  if (f.good()) {
    while (std::getline(f, buffer)) {
      std::vector<char> line(buffer.begin(), buffer.end());
      v.push_back(line);
    }
  }
  return v;
}
