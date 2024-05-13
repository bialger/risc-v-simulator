#ifndef LIBRARIES_ASM_RISCV_RISCVCOMMANDEXECUTOR_HPP_
#define LIBRARIES_ASM_RISCV_RISCVCOMMANDEXECUTOR_HPP_

#include "RISCVAssemblerCommand.hpp"

class RISCVCommandExecutor {
 public:
  RISCVCommandExecutor() = delete;
  explicit RISCVCommandExecutor(const std::vector<RISCVAssemblerCommand>& commands);

  void Execute();

  [[nodiscard]] float GetLRUHitRate() const;
  [[nodiscard]] float GetBitpLRUHitRate() const;

 private:
  std::vector<RISCVAssemblerCommand> commands_;
  ProcessMemory memory_;
  RISCVRegisters registers_;
};

#endif //LIBRARIES_ASM_RISCV_RISCVCOMMANDEXECUTOR_HPP_
