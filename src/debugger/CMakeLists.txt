INCLUDE_DIRECTORIES( ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

SET(debugger_SRCS
disassemblermodel.cpp
debuggercentralwidget.cpp
debuggermainwindow.cpp
emulatorcontroller.cpp
registerdockwidget.cpp
main.cpp
)

QT4_WRAP_UI(debugger_SRCS
debuggercentralwidget.ui
registerdockwidget.ui
)

QT4_AUTOMOC(${debugger_SRCS})

ADD_EXECUTABLE(legacyspc_debugger ${debugger_SRCS})

TARGET_LINK_LIBRARIES(legacyspc_debugger ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} legacyspc)
