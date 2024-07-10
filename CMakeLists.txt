cmake_minimum_required(VERSION 3.10)

if ("${xxhash_dev_type}" STREQUAL "BENCH")
	add_subdirectory("tests/bench")
elseif("${xxhash_dev_type}" STREQUAL "COLLISIONS")
	add_subdirectory("tests/collisions")
endif()
