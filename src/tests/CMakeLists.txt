ENABLE_TESTING()

INCLUDE_DIRECTORIES(
	"${gmock_SOURCE_DIR}/include"
	"${gmock_SOURCE_DIR}"
	"${gtest_SOURCE_DIR}/include"
)

LINK_DIRECTORIES(
	"${CMAKE_CURRENT_BINARY_DIR}/../liblegacyspc/"
)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set(LEGACYSPC_TEST_LIBRARIES gtest legacyspc)

add_definitions(-DLEGACYSPC_TESTDATA="\\"${CMAKE_CURRENT_SOURCE_DIR}/data/\\"")

FILE(GLOB legacyspc_unittest_SRCS "*.cpp")

ADD_EXECUTABLE(legacyspc_unittests ${legacyspc_unittest_SRCS})

TARGET_LINK_LIBRARIES(legacyspc_unittests ${LEGACYSPC_TEST_LIBRARIES})

ADD_TEST(legacyspc_unittests ${EXECUTABLE_OUTPUT_PATH}/legacyspc_unittests)
