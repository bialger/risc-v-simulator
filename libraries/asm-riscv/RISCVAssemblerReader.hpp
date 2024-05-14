#ifndef LIBRARIES_ASM_RISCV_RISCVASSEMBLERREADER_HPP_
#define LIBRARIES_ASM_RISCV_RISCVASSEMBLERREADER_HPP_

#include <vector>
#include <string>

#include "RISCVAssemblerCommand.hpp"

class RISCVAssemblerReader {
 public:
  explicit RISCVAssemblerReader(std::vector<std::vector<std::string>>& lines);

  void Process();

  std::vector<RISCVAssemblerCommand> GetCommands();

 private:
  std::vector<std::vector<std::string>>& lines_;
  std::vector<RISCVAssemblerCommand> commands_;

  void PreProcess();
  static int32_t GetInteger(const std::string& str);
  static RISCVAssemblerCommand ProcessLine(const std::vector<std::string>& line);
};

#endif //LIBRARIES_ASM_RISCV_RISCVASSEMBLERREADER_HPP_
