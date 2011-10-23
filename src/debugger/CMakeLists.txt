INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}/qhexedit2
	${CMAKE_CURRENT_BINARY_DIR}/qhexedit2
)

SET(debugger_SRCS
disassemblermodel.cpp
debuggercentralwidget.cpp
debuggermainwindow.cpp
emulatorcontroller.cpp
registerdockwidget.cpp
memoryviewdockwidget.cpp
main.cpp
)

SET(qhexedit2_SRCS
	qhexedit2/commands.cpp
	qhexedit2/qhexedit.cpp
	qhexedit2/qhexedit_p.cpp
	qhexedit2/xbytearray.cpp
)

QT4_AUTOMOC(${qhexedit2_SRCS})

QT4_WRAP_UI(debugger_SRCS
debuggercentralwidget.ui
registerdockwidget.ui
)

QT4_AUTOMOC(${debugger_SRCS})

ADD_EXECUTABLE(legacyspc_debugger ${debugger_SRCS} ${qhexedit2_SRCS})

TARGET_LINK_LIBRARIES(legacyspc_debugger ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} legacyspc)
