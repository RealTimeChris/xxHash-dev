cmake_minimum_required(VERSION 3.10)
project(benchHash LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Set default compiler flags
if (MSVC)
    # MSVC-specific flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /MDd /Zi /Ob0 /Od /RTC1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /MDd /Zi /Ob0 /Od /RTC1")
else()
    # Non-MSVC (GCC, Clang) specific flags
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wstrict-aliasing=1 -std=c99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wstrict-aliasing=1")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CPP_FLAGS "-I../..")

# Custom flags
if(DEFINED MOREFLAGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MOREFLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MOREFLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MOREFLAGS}")
endif()

# Source files
set(SRC_FILES main.cpp bhDisplay.cpp benchHash.cpp benchfn.cpp timefn.cpp)

if (WIN32)
	set(OS "windows")
	set(ARCHITECTURE "x64")
elseif(APPLE)
	EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
	set(OS "osx")
else()
	set(OS "linux")
	set(ARCHITECTURE "x64")
endif()

include(FetchContent)
FetchContent_Declare(
  Chrishendo
  GIT_REPOSITORY https://github.com/RealTimeChris/Chrishendo
  GIT_TAG main
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Chrishendo)

FetchContent_Declare(
  Jsonifier
  GIT_REPOSITORY https://github.com/RealTimeChris/Jsonifier
  GIT_TAG dev
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Jsonifier)

if (EXISTS "${VCPKG_DIR}")
	set(xxHash_DIR "${VCPKG_DIR}/installed/${ARCHITECTURE}-${OS}/share/xxhash")
endif()

find_package(xxHash CONFIG REQUIRED)

# Targets
add_executable(benchHash ${SRC_FILES})
add_executable(benchHash32 ${SRC_FILES})
add_executable(benchHash_avx2 ${SRC_FILES})
add_executable(benchHash_hw ${SRC_FILES})

# Target-specific compiler flags
#target_compile_options(benchHash32 PRIVATE -m32)
#target_compile_options(benchHash_avx2 PRIVATE -mavx2)
#target_compile_options(benchHash_hw PRIVATE -mavx2 -maes -DHARDWARE_SUPPORT)
#target_compile_options(benchHash_hw PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-mpclmul -std=c++14>)

# Header dependencies
add_custom_target(check_headers
  DEPENDS bhDisplay.h benchHash.h hashes.h
  COMMAND ${CMAKE_COMMAND} -E echo "Headers checked"
)
target_link_libraries(benchHash PUBLIC xxHash::xxhash Chrishendo::Chrishendo Jsonifier::Jsonifier)
target_link_libraries(benchHash32 PUBLIC xxHash::xxhash Chrishendo::Chrishendo Jsonifier::Jsonifier)
target_link_libraries(benchHash_avx2 PUBLIC xxHash::xxhash Chrishendo::Chrishendo Jsonifier::Jsonifier)
target_link_libraries(benchHash_hw PUBLIC xxHash::xxhash Chrishendo::Chrishendo Jsonifier::Jsonifier)

# Include header dependencies
add_dependencies(benchHash check_headers)
add_dependencies(benchHash32 check_headers)
add_dependencies(benchHash_avx2 check_headers)
add_dependencies(benchHash_hw check_headers)

# Clean target
add_custom_target(clean_all
  COMMAND ${CMAKE_COMMAND} -E remove -f *.o benchHash benchHash32 benchHash_avx2 benchHash_hw
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Cleaning up..."
)

# Default target
add_custom_target(default DEPENDS benchHash)
add_custom_target(all_executables DEPENDS benchHash benchHash32 benchHash_avx2 benchHash_hw)
