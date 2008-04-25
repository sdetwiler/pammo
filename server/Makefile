########################################
## Server project.
########################################


########################################
## Path configuration.
########################################

SERVER_ROOT        ?= /home/steved/prj/server
BUILD_DST         ?= $(SERVER_ROOT)/build


########################################
## Base make rules.
########################################

## SCD: need conditional test for existance of path.
#common:
#	 mkdir $(BUILD_DST)

all:	server.elf

clean:
	rm  $(BUILD_DST)/*.o $(BUILD_DST)/*.elf $(BUILD_DST)/*.so



########################################
## Common setttings for all builds.
########################################

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

SERVER_SRCS     += connection.cpp 
SERVER_SRCS     += command.cpp 
SERVER_SRCS     += session.cpp 
SERVER_SRCS     += server.cpp 
SERVER_SRCS     += gameServer.cpp 
SERVER_SRCS     += main.cpp

server.elf: 
	$(CXX) -o$(SERVER_TARGET) $(COMMON_PARAMS) $(SERVER_CXXFLAGS) $(SERVER_INCPATHS) $(SERVER_LIBPATHS) $(SERVER_DEFINES) $(SERVER_LDFLAGS) $(SERVER_LIBS) $(SERVER_SRCS)

#server: server.elf



