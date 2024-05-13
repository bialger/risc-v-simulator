#ifndef LIBRARIES_ASM_RISCV_RISCVASSEMBLERREADER_HPP_
#define LIBRARIES_ASM_RISCV_RISCVASSEMBLERREADER_HPP_

#include <vector>
#include <string>

#include "RISCVAssemblerCommand.hpp"

class RISCVAssemblerReader {
 public:
  explicit RISCVAssemblerReader(const std::string& filename);

  void Process();

  std::vector<RISCVAssemblerCommand> GetCommands();

 private:
  std::vector<std::vector<std::string>> lines_;
  std::vector<RISCVAssemblerCommand> commands_;

  void PreProcess();
  static RISCVAssemblerCommand ProcessLine(const std::vector<std::string>& line);
};

#endif //LIBRARIES_ASM_RISCV_RISCVASSEMBLERREADER_HPP_
