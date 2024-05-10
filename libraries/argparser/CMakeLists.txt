add_library(argparser STATIC ArgParser.cpp
        ArgParser.hpp
        ArgumentBuilder.hpp
        Argument.hpp
        ConcreteArgumentBuilder.hpp
        ConcreteArgument.hpp
)

add_subdirectory(basic)

target_link_libraries(argparser PRIVATE argparser_basic)

target_include_directories(argparser PUBLIC ${PROJECT_SOURCE_DIR})
