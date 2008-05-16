########################################
## PAMMO project.
########################################


########################################
## Path configuration.
########################################

PAMMO_ROOT        ?= .
BUILD_DST         ?= $(PAMMO_ROOT)/build


########################################
## Base make rules.
########################################

## SCD: need conditional test for existance of path.
#common:
#	 mkdir $(BUILD_DST)

all:	server.elf client.elf

clean:
	rm  $(BUILD_DST)/*.o $(BUILD_DST)/*.elf $(BUILD_DST)/*.so



########################################
## Common setttings for all builds.
########################################

CXX = ccache g++

COMMON_CXXFLAGS += -g
COMMON_INCPATHS += -I.
COMMON_LIBPATHS +=
COMMON_DEFINES  +=
COMMON_LDFLAGS  +=
COMMON_LIBS     +=

# All common parameters.
COMMON_PARAMS = $(COMMON_CXXFLAGS) $(COMMON_INCPATHS) $(COMMON_LIBPATHS) $(COMMON_DEFINES) $(COMMON_LDFLAGS) $(COMMON_LIBS)


########################################
## Server.
########################################

SERVER_CXXFLAGS +=
SERVER_INCPATHS +=
SERVER_LIBPATHS += -L$(BUILD_DST)
SERVER_DEFINES  +=
SERVER_LDFLAGS  += -Wl,-rpath,$(BUILD_DST),-rpath,.
SERVER_LIBS     += -lpthread
SERVER_TARGET    = $(BUILD_DST)/server.elf

SERVER_SRCS     += src/connection.cpp
SERVER_SRCS     += src/command.cpp
SERVER_SRCS     += src/session.cpp
SERVER_SRCS     += src/server.cpp
SERVER_SRCS     += src/gameServer.cpp
SERVER_SRCS     += src/serverSession.cpp
SERVER_SRCS     += src/serverMap.cpp
SERVER_SRCS     += src/serverMapIndex.cpp
SERVER_SRCS     += src/player.cpp
SERVER_SRCS     += src/types.cpp

SERVER_SRCS     += src/serverMain.cpp

server.elf: 
	$(CXX) -o$(SERVER_TARGET) $(COMMON_PARAMS) $(SERVER_CXXFLAGS) $(SERVER_INCPATHS) $(SERVER_LIBPATHS) $(SERVER_DEFINES) $(SERVER_LDFLAGS) $(SERVER_LIBS) $(SERVER_SRCS)


########################################
## Client.
########################################

CLIENT_CXXFLAGS +=
CLIENT_INCPATHS +=
CLIENT_LIBPATHS += -L$(BUILD_DST)
CLIENT_DEFINES  +=
CLIENT_LDFLAGS  += -Wl,-rpath,$(BUILD_DST),-rpath,.
CLIENT_LIBS     += -lpthread
CLIENT_TARGET    = $(BUILD_DST)/client.elf

CLIENT_SRCS     += src/connection.cpp
CLIENT_SRCS     += src/command.cpp
CLIENT_SRCS     += src/session.cpp
CLIENT_SRCS     += src/client.cpp
CLIENT_SRCS     += src/gameClient.cpp
CLIENT_SRCS     += src/clientMain.cpp
CLIENT_SRCS     += src/types.cpp

client.elf: 
	$(CXX) -o$(CLIENT_TARGET) $(COMMON_PARAMS) $(CLIENT_CXXFLAGS) $(CLIENT_INCPATHS) $(CLIENT_LIBPATHS) $(CLIENT_DEFINES) $(CLIENT_LDFLAGS) $(CLIENT_LIBS) $(CLIENT_SRCS)



