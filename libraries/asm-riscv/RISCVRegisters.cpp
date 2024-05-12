#include "RISCVRegisters.hpp"

RISCVRegisters::RISCVRegisters() : registers_(32, 0), pc_(0) {}

int32_t RISCVRegisters::GetRegister(size_t index) const {
  if (index >= registers_.size() || index == 0) {
    return 0;
  }

  return registers_[index];
}

uint32_t RISCVRegisters::GetPC() const {
  return pc_;
}

void RISCVRegisters::SetRegister(size_t index, int32_t value) {
  if (index >= registers_.size() || index == 0) {
    return;
  }

  registers_[index] = value;
}

void RISCVRegisters::SetPC(uint32_t value) {
  pc_ = value;
}
