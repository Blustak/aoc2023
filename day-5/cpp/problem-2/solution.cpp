// "Copyright 2023 anyone who wants this trash"
//
// KEY MAP INFO
// source-to-destination map:
// destination my_range start | source range start | range
//
// EXAMPLE:
// seed-to-soil map:
// 50 98 2
// 52 50 48
//
// line 1: soils [50-51] (my_range 2) map to seeds [98-99], therefore soil 50
// corresponds to seed 98, soil 51 to seed 99. line 2: soils [52-99] (my_range
// 48) map to seeds [50-97], therefore soil 52 corresponds to seed 50 ... soil
// 99 to seed 97
//
// PROBLEM 2: seed codes are written as pairs, going as code start: my_range.
//
// This implementation really sucks. I imagine some pathfinding algo would make
// this a lot faster.
//
//

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split_by_delim(const std::string &str, char delim);

struct my_range {
  std::uint32_t start;
  std::uint32_t length;
};

struct almanac_entry {
  my_range dest_range;
  my_range src_range;
  std::uint32_t length = src_range.length;
};

enum map_type {
  SEED_SOIL,
  SOIL_FERT,
  FERT_WATER,
  WATER_LIGHT,
  LIGHT_TEMP,
  TEMP_HUM,
  HUM_LOCATION,
};

enum read_state {
  SEED_CODES,
  HEADER,
  MAP,
  READ,
  ERROR,
};

int main(int argc, char **argv) {
  std::vector<std::uint32_t> seed_codes;
  std::map<map_type, std::vector<almanac_entry>> almanac;
  std::map<std::string, map_type> translator = {
      {"seed-to-soil map", map_type::SEED_SOIL},
      {"soil-to-fertilizer map", map_type::SOIL_FERT},
      {"fertilizer-to-water map", map_type::FERT_WATER},
      {"water-to-light map", map_type::WATER_LIGHT},
      {"light-to-temperature map", map_type::LIGHT_TEMP},
      {"temperature-to-humidity map", map_type::TEMP_HUM},
      {"humidity-to-location map", map_type::HUM_LOCATION}};
  std::vector<std::uint32_t> seed_to_location;
  std::string buffer;
  std::ifstream f(argv[1]);
  read_state state = read_state::READ;
  map_type m_type;
  if (f.good()) {
    bool loop = true;
    // Initialise data
    while (loop) {
      switch (state) {
      case read_state::SEED_CODES:
        for (std::string element :
             split_by_delim(buffer.substr(buffer.find(":") + 1), ' ')) {
          if (element.size() > 0 && isdigit(element[0])) {
            seed_codes.push_back(std::stoull(element));
          }
        }
        state = read_state::READ;
        break;
      case read_state::HEADER: {
        std::string head = buffer.substr(0, buffer.find(':'));
        if (head.size() > 0 && translator.find(head) != translator.end()) {
          m_type = translator[head];
          state = read_state::READ;
        } else {
          state = read_state::ERROR;
        }
        break;
      }
      case read_state::MAP: {
        std::istringstream iss(buffer);
        std::string t_buffer;
        std::vector<std::uint32_t> entries;
        entries.reserve(3);
        while (getline(iss, t_buffer, ' ')) {
          if (t_buffer.size() >= 1) {
            entries.push_back(std::stoull(t_buffer));
          }
        }
        almanac[m_type].push_back(
            almanac_entry{my_range{entries[0], entries[2]},
                          my_range{entries[1], entries[2]}});
        state = read_state::READ;
      }
      case read_state::READ:
        if (f.eof()) {
          loop = false;
          break;
        }
        std::getline(f, buffer);
        if (buffer.size() <= 1) {
          // Newline, we can read the next line.
          break;
        }
        if (buffer.find("seeds:") != std::string::npos) {
          state = read_state::SEED_CODES;
        } else if (std::all_of(buffer.begin(), buffer.end(), [](char c) {
                     return isdigit(c) || isspace(c);
                   })) {
          state = read_state::MAP;
        } else {
          state = read_state::HEADER;
        }
        break;
      case read_state::ERROR:
        std::cout << "The program encountered an error. Aborting...\n";
        loop = false;
      }
    }
    if (state == read_state::ERROR) {
      return 0;
    }
    // Algorithm
    std::uint32_t smallest_loc = uint32_t(-1);
    std::vector<my_range> ranges;
    for (int i = 0; i < seed_codes.size(); i += 2) {
      ranges.push_back(my_range{seed_codes[i], seed_codes[i + 1]});
    }
    std::vector<my_range> new_ranges;
    for (map_type m = map_type::SEED_SOIL; m <= map_type::HUM_LOCATION;
         m = map_type(m + 1)) {
      std::vector<almanac_entry> table = almanac[m];
      while (!table.empty()) {
        almanac_entry row = table.back();
        for (my_range r : ranges) {
          // no intersection; add as is
          if (row.src_range.start + row.length < r.start ||
              r.start + r.length < row.src_range.start) {
            new_ranges.push_back(r);
          } else {
            std::uint32_t intersection_start =
                std::max(row.src_range.start, r.start);
            std::uint32_t intersection_length =
                std::min(row.src_range.start + row.length, r.start + r.length) -
                intersection_start;
            // lhs of intersection
            if (row.src_range.start < r.start) {
              new_ranges.push_back(my_range{row.dest_range.start,
                                            r.start - row.src_range.start});
            } else if (r.start < row.src_range.start) {
              new_ranges.push_back(
                  my_range{r.start, row.src_range.start - r.start});
            }
            new_ranges.push_back(
                my_range{row.dest_range.start +
                             (intersection_start - row.src_range.start),
                         intersection_length});
            // rhs of intersection
            if (r.start + r.length < row.src_range.start + row.length) {
              new_ranges.push_back(my_range{
                  row.dest_range.start + (r.start + r.length),
                  row.src_range.start + row.length - (r.start + r.length)});
            } else if (row.src_range.start + row.length < r.start + r.length) {
              new_ranges.push_back(my_range{
                  r.start + (row.src_range.start + row.length),
                  r.start + r.length - (row.src_range.start + row.length)});
            }
          }
        }
        table.pop_back();
      }
      ranges = new_ranges;
      new_ranges.clear();
    }

    for (auto r : ranges) {
      if (r.start < smallest_loc) {
        smallest_loc = r.start;
      }
    }
    std::cout << "The smallest location I found was " << smallest_loc << ".\n";
  }
  return 0;
}

std::vector<std::string> split_by_delim(const std::string &str, char delim) {
  std::vector<std::string> members;
  std::stringstream iss(str);
  std::string buffer;

  while (std::getline(iss, buffer, delim)) {
    members.push_back(buffer);
  }
  return members;
}
