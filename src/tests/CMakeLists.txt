ENABLE_TESTING()

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../liblegacyspc/ ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} )
LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/../liblegacyspc/ )

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set(LEGACYSPC_TEST_LIBRARIES ${QT_QTTEST_LIBRARY} legacyspc)

add_definitions(-DLEGACYSPC_TESTDATA="\\"${CMAKE_CURRENT_SOURCE_DIR}/data/\\"")

### TestDataTransmissionOperations
SET(testDataTransmissionOperations_SRCS testdatatransmissionoperations.cpp commandtestbase.cpp)

QT4_AUTOMOC( ${testDataTransmissionOperations_SRCS} )

ADD_EXECUTABLE(testDataTransmissionOperations ${testDataTransmissionOperations_SRCS})

TARGET_LINK_LIBRARIES(testDataTransmissionOperations ${LEGACYSPC_TEST_LIBRARIES})

ADD_TEST(testDataTransmissionOperations ${EXECUTABLE_OUTPUT_PATH}/testDataTransmissionOperations)

### TestArithmeticOperations
SET(testArithmeticOperations_SRCS testarithmeticoperations.cpp commandtestbase.cpp)

QT4_AUTOMOC( ${testArithmeticOperations_SRCS} )

ADD_EXECUTABLE(testArithmeticOperations ${testArithmeticOperations_SRCS} )

TARGET_LINK_LIBRARIES(testArithmeticOperations ${LEGACYSPC_TEST_LIBRARIES} )

ADD_TEST(testArithmeticOperations ${EXECUTABLE_OUTPUT_PATH}/testArithmeticOperations)

### TestLogicOperations
SET(testLogicOperations_SRCS testlogicoperations.cpp commandtestbase.cpp)

QT4_AUTOMOC( ${testLogicOperations_SRCS} )

ADD_EXECUTABLE(testLogicOperations ${testLogicOperations_SRCS} )

TARGET_LINK_LIBRARIES(testLogicOperations ${LEGACYSPC_TEST_LIBRARIES} )

ADD_TEST(testLogicOperations ${EXECUTABLE_OUTPUT_PATH}/testLogicOperations)

### TestSpcFileLoader
SET(testSpcFileLoader_SRCS testspcfileloader.cpp)

QT4_AUTOMOC( ${testSpcFileLoader_SRCS} )

ADD_EXECUTABLE(testSpcFileLoader ${testSpcFileLoader_SRCS})

TARGET_LINK_LIBRARIES(testSpcFileLoader ${LEGACYSPC_TEST_LIBRARIES})

ADD_TEST(testSpcFileLoader ${EXECUTABLE_OUTPUT_PATH}/testSpcFileLoader)
