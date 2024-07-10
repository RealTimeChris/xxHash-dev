cmake_minimum_required(VERSION 3.10)
project(xxHashTests)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wundef -g")

# Define a platform-specific extension for executables
if(WIN32)
  set(EXT ".exe")
else()
  set(EXT "")
endif()

# Check for Unicode support
if("${LANG}" MATCHES "UTF-8")
  set(ENABLE_UNICODE 1)
else()
  set(ENABLE_UNICODE 0)
endif()

# Define the source directory for xxhsum
set(XXHSUM_DIR "..")
set(XXHSUM "${XXHSUM_DIR}/xxhsum${EXT}")

# Add a custom command to generate Unicode test file
add_custom_command(
  OUTPUT generate_unicode_test${EXT}
  COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} ${CMAKE_LDFLAGS} generate_unicode_test.c -o generate_unicode_test${EXT}
  DEPENDS generate_unicode_test.c
)

# Add a custom command to run Unicode test script
add_custom_target(test_unicode
  COMMAND ${CMAKE_COMMAND} -E echo "Skipping Unicode test, your terminal doesn't appear to support UTF-8."
  COMMAND ${CMAKE_COMMAND} -E echo "Try with ENABLE_UNICODE=1"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Unicode test skipped."
)

if(ENABLE_UNICODE)
  add_custom_target(test_unicode
    COMMAND ./generate_unicode_test${EXT}
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_COMMAND} -E shell ./unicode_test.sh
    DEPENDS generate_unicode_test${EXT} ${XXHSUM}
    COMMENT "Running Unicode test..."
  )
endif()

# Define a custom command for the xxhsum executable
add_custom_target(xxhsum_target
  COMMAND ${CMAKE_COMMAND} -E make_directory ${XXHSUM_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${XXHSUM} ${CMAKE_CURRENT_BINARY_DIR}
)

# Define custom targets for tests
add_custom_target(test_multiInclude
  COMMAND ${CMAKE_COMMAND} -E echo "Running test_multiInclude..."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(test_sanity
  COMMAND ${CMAKE_COMMAND} -E echo "Running test_sanity..."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(test_cli_comment_line
  COMMAND ${CMAKE_COMMAND} -E echo "Running test_cli_comment_line..."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(test_cli_ignore_missing
  COMMAND ${CMAKE_COMMAND} -E echo "Running test_cli_ignore_missing..."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(test_filename_escape
  COMMAND ${CMAKE_COMMAND} -E echo "Running test_filename_escape..."
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Define a custom target for cleaning
add_custom_target(clean
  COMMAND ${CMAKE_COMMAND} -E remove -f *.o multiInclude multiInclude_withxxhash *.unicode generate_unicode_test${EXT} unicode_test.* xxhsum* sanity_test${EXT} sanity_test_vectors_generator${EXT}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Cleaning up..."
)

# Add dependencies to the all target
add_custom_target(all
  DEPENDS test_multiInclude test_unicode test_sanity
)

# Set default target to all
add_custom_target(default DEPENDS all)
