add_library(asm_riscv STATIC
    RISCVAssemblerReader.cpp
    RISCVAssemblerReader.hpp
        RISCVComandExecutor.cpp
        RISCVComandExecutor.hpp
        RISCVAssemblerToBinary.cpp
        RISCVAssemblerToBinary.hpp
        RISCVAssemblerCommand.cpp
        RISCVAssemblerCommand.hpp
        RISCVRegisters.cpp
        RISCVRegisters.hpp
        ProcessMemory.cpp
        ProcessMemory.hpp
)