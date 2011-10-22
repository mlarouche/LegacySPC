PROJECT(LegacySPC)

# include specific modules
SET (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5 FATAL_ERROR)

# Always include srcdir and builddir in include path
# This saves typing ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY} in about every subdir
# since cmake 2.4.0
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE(MacroCheckGccVisibility)
INCLUDE(TestBigEndian)

TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
MESSAGE(STATUS "Is big-endian: ${IS_BIG_ENDIAN}")

IF(NOT IS_BIG_ENDIAN)
	ADD_DEFINITIONS(-DLEGACYSPC_LSB)
ENDIF(NOT IS_BIG_ENDIAN)

# check for visibility support
MACRO_CHECK_GCC_VISIBILITY(__LEGACYSPC_HAVE_GCC_VISIBILITY)

# Add uninstall command
CONFIGURE_FILE(
  "${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

ADD_SUBDIRECTORY( gmock )
ADD_SUBDIRECTORY( src )
