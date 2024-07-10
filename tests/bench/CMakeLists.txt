cmake_minimum_required(VERSION 3.10)
project(benchHash LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

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

add_executable(benchHash ${SRC_FILES})
add_executable(benchHash32 ${SRC_FILES})
add_executable(benchHash_avx2 ${SRC_FILES})
add_executable(benchHash_hw ${SRC_FILES})

target_compile_definitions(benchHash PUBLIC "ROOT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
target_compile_definitions(benchHash32 PUBLIC "ROOT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
target_compile_definitions(benchHash_avx2 PUBLIC "ROOT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
target_compile_definitions(benchHash_hw PUBLIC "ROOT_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\"")

target_link_libraries(benchHash PUBLIC xxHash::xxhash Chrishendo::Chrishendo Jsonifier::Jsonifier)
target_link_libraries(benchHash32 PUBLIC xxHash::xxhash Chrishendo::Chrishendo Jsonifier::Jsonifier)
target_link_libraries(benchHash_avx2 PUBLIC xxHash::xxhash Chrishendo::Chrishendo Jsonifier::Jsonifier)
target_link_libraries(benchHash_hw PUBLIC xxHash::xxhash Chrishendo::Chrishendo Jsonifier::Jsonifier)
