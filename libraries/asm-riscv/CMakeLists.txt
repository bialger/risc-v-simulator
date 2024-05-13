add_library(asm_riscv STATIC
        RISCVAssemblerReader.cpp
        RISCVAssemblerReader.hpp
        RISCVCommandExecutor.cpp
        RISCVCommandExecutor.hpp
        RISCVAssemblerToBinary.cpp
        RISCVAssemblerToBinary.hpp
        RISCVAssemblerCommand.cpp
        RISCVAssemblerCommand.hpp
        RISCVRegisters.cpp
        RISCVRegisters.hpp
        ProcessMemory.cpp
        ProcessMemory.hpp
)

target_link_libraries(asm_riscv PUBLIC
        cache
)

target_include_directories(asm_riscv PUBLIC
        ${PROJECT_SOURCE_DIR}
)
