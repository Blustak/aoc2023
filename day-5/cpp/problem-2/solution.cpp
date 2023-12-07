// "Copyright 2023 anyone who wants this trash"
//
// KEY MAP INFO
// source-to-destination map:
// destination range start | source range start | range
//
// EXAMPLE:
// seed-to-soil map:
// 50 98 2
// 52 50 48
//
// line 1: soils [50-51] (range 2) map to seeds [98-99], therefore soil 50
// corresponds to seed 98, soil 51 to seed 99. line 2: soils [52-99] (range 48)
// map to seeds [50-97], therefore soil 52 corresponds to seed 50 ... soil 99 to
// seed 97
//

#include <algorithm>
#include <any>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split_by_delim(const std::string &str, char delim);

struct almanac_entry {
  std::uint64_t dest_start;
  std::uint64_t src_start;
  std::uint64_t range;
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
  std::vector<std::uint64_t> seed_codes;
  std::map<map_type, std::vector<almanac_entry>> almanac;
  std::map<std::string, map_type> translator = {
      {"seed-to-soil map", map_type::SEED_SOIL},
      {"soil-to-fertilizer map", map_type::SOIL_FERT},
      {"fertilizer-to-water map", map_type::FERT_WATER},
      {"water-to-light map", map_type::WATER_LIGHT},
      {"light-to-temperature map", map_type::LIGHT_TEMP},
      {"temperature-to-humidity map", map_type::TEMP_HUM},
      {"humidity-to-location map", map_type::HUM_LOCATION}};
  std::vector<std::uint64_t> seed_to_location;
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
             split_by_delim(buffer.substr(buffer.find(":")), ' ')) {
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
        std::vector<std::uint64_t> entries;
        entries.reserve(3);
        while (getline(iss, t_buffer, ' ')) {
          if (t_buffer.size() >= 1) {
            entries.push_back(std::stoull(t_buffer));
          }
        }
        almanac[m_type].push_back(
            almanac_entry{entries[0], entries[1], entries[2]});
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
    for (auto seed : seed_codes) {
      std::uint64_t prev_code = seed;
      for (map_type m = map_type::SEED_SOIL; m <= map_type::HUM_LOCATION;
           m = map_type(m + 1)) {
        std::vector<almanac_entry> table = almanac[m];
        std::uint64_t t = prev_code;
        for (almanac_entry e : table) {
          if (prev_code >= e.src_start && prev_code < (e.src_start + e.range)) {
            prev_code = e.dest_start + (prev_code - e.src_start);
            break;
          }
        }
        std::cout << t << "->" << prev_code << "\n";
      }
      seed_to_location.push_back(prev_code);
      std::cout << "Prev code: " << prev_code << "\n";
    }
    std::uint64_t smallest_loc = uint64_t(-1);
    for (auto code : seed_to_location) {
      if (code < smallest_loc) {
        smallest_loc = code;
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
