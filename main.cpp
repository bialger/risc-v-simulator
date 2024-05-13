#include <iostream>
#include <filesystem>

#include "libraries/argparser/ArgParser.hpp"
#include "libraries/asm-riscv/RISCVCommandExecutor.hpp"
#include "libraries/asm-riscv/RISCVAssemblerReader.hpp"
#include "libraries/asm-riscv/RISCVAssemblerToBinary.hpp"

int main(int argc, char** argv) {
  ArgumentParser::ArgParser parser = ArgumentParser::ArgParser("RISC-V Pseudo-emulator");
  parser.AddIntArgument('r',
                        "replacement",
                        "Replacement policy: 0 - all, 1 - LRU, 2 - bit-pLRU").AddIsGood([](std::string& value) {
    return value == "0" || value == "1" || value == "2";
  }).Default(0);
  parser.AddCompositeArgument('a',
                              "asm",
                              "Assembly file").AddValidate(&ArgumentParser::IsValidFilename).AddIsGood(&ArgumentParser::IsRegularFile);
  parser.AddCompositeArgument('b', "bin", "Binary file").AddValidate(&ArgumentParser::IsValidFilename).AddIsGood([](std::string& value) {
    return !ArgumentParser::IsRegularFile(value);
  }).Default(".");
  parser.AddHelp('h', "help", "RISC-V pseudo-emulator with cache hit counter.");

  if (!parser.Parse(argc, argv, {std::cerr, true})) {
    std::cout << parser.HelpDescription() << std::endl;
    return 1;
  }

  if (parser.Help()) {
    std::cout << parser.HelpDescription() << std::endl;
    return 0;
  }

  RISCVAssemblerReader reader = RISCVAssemblerReader(parser.GetCompositeValue("asm"));

  try {
    reader.Process();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  RISCVCommandExecutor executor = RISCVCommandExecutor(reader.GetCommands());

  try {
    executor.Execute();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}