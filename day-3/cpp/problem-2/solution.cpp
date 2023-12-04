// "Copyright 2023 Blustak"
// Very naive solution.I need to look up 2d pathfinding stuff.
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

struct walkable_dimensions {
  bool left;
  bool right;
  bool up;
  bool down;
};

const char gear_char = '*';

std::vector<std::vector<char>> read_file(const std::string &file_name);

struct coordinate {
  int x;
  int y;
};
struct symbol_pos {
  coordinate c;
  char symbol;
};

bool coordinate_is_equal(const coordinate &a, const coordinate &b);

int main(int argc, char **argv) {
  std::vector<std::vector<char>> arena = read_file(argv[1]);
  std::vector<int> valid_parts;
  auto comp = [](const coordinate &c1, const coordinate &c2) {
    return c1.x < c2.x || (c1.x == c2.x && c1.y < c2.y);
  };
  std::map<coordinate, std::vector<int>, decltype(comp)> gears(comp);
  int x_left_bound = 0;
  int x_right_bound;
  int y_top_bound = 0;
  int y_bottom_bound = arena.size();

  // scan for digit
  for (int y = 0; y < y_bottom_bound; y++) {
    x_right_bound = arena[y].size() - 1;
    for (int x = 0; x < x_right_bound; x++) {
      if (isdigit(arena[y][x])) {
        std::string number;
        int x_offset = 0;
        std::vector<symbol_pos> adj_symbols;
        bool above = y - 1 > 0;
        bool below = y + 1 < y_bottom_bound;
        // add the lefthand symbols
        if (x - 1 > 0) {
          adj_symbols.push_back(
              symbol_pos{coordinate{x - 1, y}, arena[y][x - 1]});
          if (above) {
            adj_symbols.push_back(
                symbol_pos{coordinate{x - 1, y - 1}, arena[y - 1][x - 1]});
          }
          if (below) {
            adj_symbols.push_back(
                symbol_pos{coordinate{x - 1, y + 1}, arena[y + 1][x - 1]});
          }
        }
        while (x + x_offset < x_right_bound &&
               isdigit(arena[y][x + x_offset])) {
          number += arena[y][x + x_offset];
          if (above) {
            adj_symbols.push_back(symbol_pos{coordinate{x + x_offset, y - 1},
                                             arena[y - 1][x + x_offset]});
          }
          if (below) {
            adj_symbols.push_back(symbol_pos{coordinate{x + x_offset, y + 1},
                                             arena[y + 1][x + x_offset]});
          }
          x_offset++;
        }
        // add the righthand symbols
        if (x + x_offset < x_right_bound) {
          adj_symbols.push_back(
              symbol_pos{coordinate{x + x_offset, y}, arena[y][x + x_offset]});
          if (above) {
            adj_symbols.push_back(symbol_pos{coordinate{x + x_offset, y - 1},
                                             arena[y - 1][x + x_offset]});
          }
          if (below) {
            adj_symbols.push_back(symbol_pos{coordinate{x + x_offset, y + 1},
                                             arena[y + 1][x + x_offset]});
          }
        }
        x = x + x_offset;

        // Now do the adding logic
        for (symbol_pos el : adj_symbols) {
          if (el.symbol == gear_char) {
            gears[el.c].push_back(std::stoi(number));
          }
        }
      }
    }
  }

  unsigned int sum = 0;
  for (std::map<coordinate, std::vector<int>>::iterator iter = gears.begin();
       iter != gears.end(); iter++) {
    std::vector<int> v = iter->second;
    if (v.size() == 2) {
      sum += v[0] * v[1];
    }
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

bool coordinate_is_equal(const coordinate &a, const coordinate &b) {
  return (a.x == b.x && a.y == b.y);
}
