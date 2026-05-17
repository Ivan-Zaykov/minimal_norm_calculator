cmake_minimum_required(VERSION 3.15)
project(HadamardSimplex)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Режим сборки (по умолчанию Release, так как для 31-мерного пространства оптимизация в Debug будет медленной)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Настройка флагов для Debug и Release
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0 -fno-inline -Wall)
else()
    add_compile_options(-O3 -Wall)
endif()

# Находим зависимости
find_package(Eigen3 REQUIRED)
find_package(ortools REQUIRED)

# Сборка
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${EIGEN3_LIBRARIES})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Eigen3::Eigen
        ortools::ortools
)

# Форматирование кода
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE SOURCE_FILES
            "${CMAKE_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_SOURCE_DIR}/include/*.h"
    )
    add_custom_target(format
            COMMAND ${CLANG_FORMAT} -i ${SOURCE_FILES}
            COMMENT "Formatting source code"
            VERBATIM
    )
endif()

# Очистка
add_custom_target(distclean
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
        COMMENT "Removing all build artifacts"
)