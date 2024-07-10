cmake_minimum_required(VERSION 3.10)
project(CollisionTest)

# Set the C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}"  "${CMAKE_CURRENT_SOURCE_DIR}/allcodecs")

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
#set(CMAKE_CPP_FLAGS "-I../..")

# Source files
set(HASH_SRC "main.cpp;pool.cpp;sort.cpp;threading.cpp;${CMAKE_CURRENT_SOURCE_DIR}/allcodecs/dummy.cpp")
list(SORT HASH_SRC)
message("HASH SOURCE:${HASH_SRC}")
include(FetchContent)
FetchContent_Declare(
  Chrishendo
  GIT_REPOSITORY https://github.com/RealTimeChris/chrishendo
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

if (EXISTS "${VCPKG_DIR}")
	set(xxHash_DIR "${VCPKG_DIR}/installed/${ARCHITECTURE}-${OS}/share/xxhash")
endif()

find_package(xxHash CONFIG REQUIRED)


add_executable(collisionsTest ${HASH_SRC})

target_link_libraries(collisionsTest PUBLIC Chrishendo::Chrishendo Jsonifier::Jsonifier xxHash::xxhash)

# Release configuration
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

# Debug configuration
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0 -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -O0 -DDEBUG")

# Targets
#add_custom_target(release
    #COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Release
    #DEPENDS collisionsTest
#)

#add_custom_target(debug
    #COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Debug
    #DEPENDS collisionsTest
#)

# Test target
#add_custom_target(test
    #COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --config Debug
    #COMMAND ./collisionsTest --nbh=${TESTHASHES}
    #COMMAND ./collisionsTest --nbh=${TESTHASHES} --filter
    #COMMAND ./collisionsTest --nbh=${TESTHASHES} --threadlog=1
#)

# Clean target
#add_custom_target(clean_all
    #COMMAND ${CMAKE_COMMAND} -E remove -f *.o allcodecs/*.o collisionsTest
#)
