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
    x_right_bound = arena[y].size();
    for (int x = 0; x < x_right_bound; x++) {
      if (isdigit(arena[y][x])) {
        // create the number;
        int x_offset = 0;
        std::string number;
        while (x + x_offset < x_right_bound &&
               isdigit(arena[y][x + x_offset])) {
          number += arena[y][x + x_offset];
          x_offset++;
        }
        // check to see if its valid.
        bool valid_part = false;
        walkable_dimensions d = {
            .left = x > x_left_bound,
            .right = x + x_offset<x_right_bound, .up = y> y_top_bound,
            .down = y + 1 < y_bottom_bound};

        if (d.up) {
          for (int i = x; i < x + x_offset; i++) {
            if (arena[y - 1][i] != skip_char) {
              std::cout << "Found " << arena[y - 1][i] << "at y:" << y - 1
                        << ", x:" << i << "\n";
              valid_part = true;
            }
          }
        }
        if (d.down) {
          for (int i = x; i < x + x_offset; i++) {
            if (arena[y + 1][i] != skip_char) {
              std::cout << "Found " << arena[y + 1][i] << "at y:" << y + 1
                        << ", x:" << i << "\n";

              valid_part = true;
            }
          }
        }
        if (d.left) {
          if (d.up) {
            if (arena[y - 1][x - 1] != skip_char) {
              std::cout << "Found " << arena[y - 1][x - 1] << "at y:" << y - 1
                        << ", x:" << x - 1 << "\n";
              valid_part = true;
            }
          }
          if (d.down) {
            if (arena[y + 1][x - 1] != skip_char) {
              std::cout << "Found " << arena[y + 1][x - 1] << "at y:" << y + 1
                        << ", x:" << x - 1 << "\n";
              valid_part = true;
            }
          }
          if (arena[y][x - 1] != skip_char) {
            std::cout << "Found " << arena[y][x - 1] << "at y:" << y
                      << ", x:" << x - 1 << "\n";
            valid_part = true;
          }
        }
        if (d.right) {
          if (d.up) {
            if (arena[y - 1][x + x_offset] != skip_char) {
              std::cout << "Found " << arena[y - 1][x + x_offset]
                        << "at y:" << y - 1 << ", x:" << x + x_offset << "\n";
              valid_part = true;
            }
          }
          if (d.down) {
            if (arena[y + 1][x + x_offset] != skip_char) {
              std::cout << "Found " << arena[y + 1][x + x_offset]
                        << "at y:" << y + 1 << ", x:" << x + x_offset << "\n";
              valid_part = true;
            }
          }
          if (arena[y][x + x_offset] != skip_char) {
            std::cout << "Found " << arena[y][x + x_offset] << "at y:" << y
                      << ", x:" << x + x_offset << "\n";
            valid_part = true;
          }
        }
        if (valid_part) {
          std::cout << "Adding number: " << number << "\n"
                    << "Found at x:" << x << ", y:" << y << "\n";
          valid_parts.push_back(std::stoi(number));
        }
        x = x + x_offset;
      }
    }
  }
  std::cout << "The sum i got was "
            << std::reduce(valid_parts.begin(), valid_parts.end()) << "\n";
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
