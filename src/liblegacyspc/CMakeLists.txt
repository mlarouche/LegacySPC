SET(liblegacyspc_SRCS
debuggerspcrunner.cpp
memorymap.cpp
processor.cpp
ram.cpp
spccomponentmanager.cpp
spcfile.cpp
spcfileloader.cpp
spcfilememoryloader.cpp
spcrunner.cpp
)

ADD_LIBRARY(legacyspc SHARED ${liblegacyspc_SRCS})

# TODO install
install(TARGETS legacyspc DESTINATION lib )
