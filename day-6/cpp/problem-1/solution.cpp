// "Copyright 2023 Blu"
#include <algorithm>
#include <any>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> read_lines(const std::string &file_path);
int get_midpoint(const int &race_time);
int get_solution(const int &race_time, const int &record_time);
static inline void rtrim(std::string &s);
static inline void ltrim(std::string &s);
static inline void trim(std::string &s);

int main(int argc, char **argv) {
  std::vector<std::string> lines = read_lines(argv[1]);
  if (lines.empty()) {
    std::cout << "Sorry, file was not read properly. Aborting...\n";
    return 5; // IO problem
  }
  std::vector<std::pair<int, int>> times_records;
  {
    std::vector<int> times;
    std::vector<int> records;
    std::string buffer;
    for (auto line : lines) {
      if (line.find("Time:") != std::string::npos) {
        line.erase(0, line.find("Time:") + 5);
        std::istringstream iss(line);
        while (std::getline(iss, buffer, ' ')) {
          trim(buffer);
          if (buffer.size() > 0) {
            times.push_back(std::stoi(buffer));
          }
        }
      } else if (line.find("Distance:") != std::string::npos) {
        line.erase(0, line.find("Distance:") + 9);
        std::istringstream iss(line);
        while (std::getline(iss, buffer, ' ')) {
          trim(buffer);
          if (buffer.size() > 0) {
            records.push_back(std::stoi(buffer));
          }
        }
      }
    }
    for (int i = 0; i < times.size(); i++) {
      times_records.push_back(std::make_pair(times[i], records[i]));
    }
  }
  int prod = 1;
  for (auto p : times_records) {
    int sol = get_solution(p.first, p.second);
    std::cout << "Solution for (" << p.first << "," << p.second << "): " << sol
              << "\n";
    if (sol > 0) {
      prod *= sol;
    }
  }
  std::cout << "My final product was " << prod << ". \n";
  return 0;
}

std::vector<std::string> read_lines(const std::string &file_path) {
  std::vector<std::string> v;
  std::ifstream f(file_path.c_str());
  if (f.good()) {
    std::string buffer;
    while (std::getline(f, buffer)) {
      trim(buffer);
      v.push_back(buffer);
    }
  }
  return v;
}

int get_midpoint(const int &race_time) {
  return ((race_time % 2 == 1) ? (race_time + 1) : race_time) / 2;
}

int get_solution(const int &race_time, const int &record_time) {
  // returns the number of input times that will beat the record time (an
  // integer value.)
  int solution;
  int mp = get_midpoint(race_time);
  // Eqn: 2*(MP - CEIL((RaceT - sqrt(RaceT^2 - 4*RecT)/2)))
  solution =
      2 * (mp - (std::ceil((race_time - std::sqrt((race_time * race_time) -
                                                  4 * record_time)) /
                           2)));
  if (race_time % 2 == 0 && solution > 0) {
    solution -= 1;
  } // Remove point counted twice.}
  return solution;
}

// Copied unceremoniously from
// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring, thanks
// Evan Teran!
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

static inline void trim(std::string &s) {
  rtrim(s);
  ltrim(s);
}
