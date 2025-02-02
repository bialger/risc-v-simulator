#include <iterator>
#include <limits>
#include <algorithm>
#include <stdexcept>

#include "RISCVAssemblerReader.hpp"

RISCVAssemblerReader::RISCVAssemblerReader(std::vector<std::vector<std::string>>& lines) : lines_(lines), commands_() {}

void RISCVAssemblerReader::PreProcessLine(std::vector<std::string>& line) {
  for (auto& word : line) {
    if (word.back() == ',') {
      word.pop_back();
    }

    std::transform(word.begin(), word.end(), word.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    for (auto it = word.begin(); it != word.end(); ++it) {
      if (*it == '(') {
        std::string new_word = word.substr(it - word.begin() + 1, word.end() - it - 2);
        word.erase(it, word.end());
        line.push_back(new_word);
        break;
      }
    }
  }

  for (auto& word : line) {
    if (word == "zero") {
      word = "x0";
    } else if (word == "ra") {
      word = "x1";
    } else if (word == "sp") {
      word = "x2";
    } else if (word == "gp") {
      word = "x3";
    } else if (word == "tp") {
      word = "x4";
    } else if (word == "t0") {
      word = "x5";
    } else if (word == "t1") {
      word = "x6";
    } else if (word == "t2") {
      word = "x7";
    } else if (word == "s0" || word == "fp") {
      word = "x8";
    } else if (word == "s1") {
      word = "x9";
    } else if (word == "a0") {
      word = "x10";
    } else if (word == "a1") {
      word = "x11";
    } else if (word == "a2") {
      word = "x12";
    } else if (word == "a3") {
      word = "x13";
    } else if (word == "a4") {
      word = "x14";
    } else if (word == "a5") {
      word = "x15";
    } else if (word == "a6") {
      word = "x16";
    } else if (word == "a7") {
      word = "x17";
    } else if (word == "s2") {
      word = "x18";
    } else if (word == "s3") {
      word = "x19";
    } else if (word == "s4") {
      word = "x20";
    } else if (word == "s5") {
      word = "x21";
    } else if (word == "s6") {
      word = "x22";
    } else if (word == "s7") {
      word = "x23";
    } else if (word == "s8") {
      word = "x24";
    } else if (word == "s9") {
      word = "x25";
    } else if (word == "s10") {
      word = "x26";
    } else if (word == "s11") {
      word = "x27";
    } else if (word == "t3") {
      word = "x28";
    } else if (word == "t4") {
      word = "x29";
    } else if (word == "t5") {
      word = "x30";
    } else if (word == "t6") {
      word = "x31";
    }
  }
}

void RISCVAssemblerReader::PreProcess() {
  for (auto& line : lines_) {
    for (auto it = line.begin(); it != line.end(); ++it) {
      if (*it == "#" || ((*it)[0] == '/' && (*it)[1] == '/')) {
        line.erase(it, line.end());
        break;
      }
    }
  }

  for (auto it = lines_.begin(); it != lines_.end(); ++it) {
    if (it->empty()) {
      lines_.erase(it);
      --it;
    }
  }

  for (auto& line : lines_) {
    PreProcessLine(line);
  }
}

RISCVAssemblerCommand RISCVAssemblerReader::ProcessLine(const std::vector<std::string>& line) {
  uint8_t reg1 = 0;
  uint8_t reg2 = 0;
  int32_t value = 0;

  if (line.size() == 2) {
    if (line[1][0] == 'x') {
      reg1 = std::stoi(line[1].substr(1));
    } else {
      value = GetInteger(line[1]);
    }
  } else if (line.size() == 3) {
    reg1 = std::stoi(line[1].substr(1));

    if (line[2][0] == 'x') {
      reg2 = std::stoi(line[2].substr(1));
    } else {
      value = GetInteger(line[2]);
    }
  } else if (line.size() == 4) {
    reg1 = std::stoi(line[1].substr(1));

    if (line[2][0] == 'x') {
      reg2 = std::stoi(line[2].substr(1));

      if (line[3][0] == 'x') {
        value = std::stoi(line[3].substr(1));
      } else {
        value = GetInteger(line[3]);
      }
    } else {
      value = GetInteger(line[2]);

      if (line[3][0] == 'x') {
        reg2 = std::stoi(line[3].substr(1));
      }
    }
  }

  return {line[0], reg1, reg2, value};
}

void RISCVAssemblerReader::Process() {
  PreProcess();

  for (const auto& line : lines_) {
    commands_.push_back(ProcessLine(line));
  }
}

std::vector<RISCVAssemblerCommand> RISCVAssemblerReader::GetCommands() {
  return commands_;
}

int32_t RISCVAssemblerReader::GetInteger(const std::string& str) {
  int32_t save_errno = errno;
  errno = 0;
  char* end;
  int64_t pre_value = std::strtoll(str.c_str(), &end, 0);

  if (end == str.c_str() || *end != '\0' ||
      ((pre_value <= std::numeric_limits<int32_t>::min() || pre_value >= std::numeric_limits<int32_t>::max()) &&
          errno == ERANGE)) {
    errno = save_errno;
    throw std::runtime_error("Invalid integer " + str);
  } else {
    return static_cast<int32_t>(pre_value);
  }
}
