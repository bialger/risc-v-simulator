#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>

#include "libraries/argparser/ArgParser.hpp"
#include "libraries/asm-riscv/RISCVCommandExecutor.hpp"
#include "libraries/asm-riscv/RISCVAssemblerReader.hpp"
#include "libraries/asm-riscv/RISCVAssemblerToBinary.hpp"

int main(int argc, char** argv) {
  ArgumentParser::ArgParser parser = ArgumentParser::ArgParser("RISC-V Pseudo-emulator");
  CompositeString default_bin = "--DEFAULT--BIN--";
  parser.AddIntArgument('r',
                        "replacement",
                        "Replacement policy: 0 - all, 1 - LRU, 2 - bit-pLRU").AddIsGood([](std::string& value) {
    return value == "0" || value == "1" || value == "2";
  }).Default(0);
  parser.AddCompositeArgument('a',
                              "asm",
                              "Assembly input_file").AddValidate(&ArgumentParser::IsValidFilename).AddIsGood(&ArgumentParser::IsRegularFile);
  parser.AddCompositeArgument('b', "bin", "Binary input_file").AddValidate(&ArgumentParser::IsValidFilename).AddIsGood([](std::string& value) {
    return !ArgumentParser::IsDirectory(value);
  }).Default(".");
  parser.AddHelp('h', "help", "RISC-V pseudo-emulator with cache_ hit counter.");

  if (!parser.Parse(argc, argv, {std::cerr, true})) {
    std::cout << parser.HelpDescription() << std::endl;
    return 1;
  }

  if (parser.Help()) {
    std::cout << parser.HelpDescription() << std::endl;
    return 0;
  }

  std::string filename = parser.GetCompositeValue("asm");
  std::ifstream input_file(filename);
  std::vector<std::vector<std::string>> lines;
  std::string line;

  if (!input_file.is_open()) {
    std::cerr << "Failed to open input_file: " + filename << std::endl;
    return 1;
  }

  while (std::getline(input_file, line)) {
    std::istringstream iss(line);
    lines.emplace_back(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>());
  }

  RISCVAssemblerReader reader = RISCVAssemblerReader(lines);

  try {
    reader.Process();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  if (parser.GetCompositeValue("bin") != default_bin) {
    std::string filename = parser.GetCompositeValue("bin");
    std::ofstream output_file(filename);
    std::vector<uint32_t> commands_code = RISCVAssemblerToBinary::Assemble(reader.GetCommands());

    for (uint32_t command : commands_code) {
      char data[4];
      data[3] = static_cast<char>((command >> 24) & 0xFF);
      data[2] = static_cast<char>((command >> 16) & 0xFF);
      data[1] = static_cast<char>((command >> 8) & 0xFF);
      data[0] = static_cast<char>(command & 0xFF);
      output_file.write(data, 4);
    }
  }

  RISCVCommandExecutor executor = RISCVCommandExecutor(reader.GetCommands());

  try {
    executor.Execute();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  int32_t policy = parser.GetIntValue("replacement");
  float lru_rate = executor.GetLRUHitRate() * 100;
  float bitp_lru_rate = executor.GetBitpLRUHitRate() * 100;

  if (policy == 0) {
    printf("LRU\thit rate: %3.4f%%\npLRU\thit rate: %3.4f%%\n", lru_rate, bitp_lru_rate);
  } else if (policy == 1) {
    printf("LRU\thit rate: %3.4f%%\n", lru_rate);
  } else if (policy == 2) {
    printf("pLRU\thit rate: %3.4f%%\n", bitp_lru_rate);
  }

  return 0;
}