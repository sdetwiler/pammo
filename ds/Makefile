#CC=g++-3.3
DSROOT=.

OUTNAME=$(DSROOT)/libds

INCDIRS+= /usr/include/mysql
#INCDIRS+= $(DSROOT)
#INCDIRS+= $(DSROOT)/database $(DSROOT)/database/mysql
LIBS+=

SRCS+= xmlElement.cpp 
SRCS+= field.cpp 
SRCS+= fieldString.cpp fieldInt.cpp 
SRCS+= fieldFactory.cpp

SRCS+= table.cpp

SRCS+= db/server.cpp
SRCS+= db/serverFactory.cpp
SRCS+= db/connectionPool.cpp
SRCS+= db/database.cpp
SRCS+= db/query.cpp


SRCS+= db/mysql/server.cpp
SRCS+= db/mysql/query.cpp
SRCS+= db/mysql/database.cpp

SRCS+= object.cpp
SRCS+= objectTemplate.cpp
SRCS+= objectTemplateIndex.cpp

SRCS+= user.cpp
SRCS+= session.cpp
SRCS+= sessionIndex.cpp

SRCS+= dataStore.cpp

SRCS+= log.cpp


#MySQL config script
MYSQLCFG=mysql_config


#CFLAGS+= -c -fpic
#CFLAGS+= -Idatabase -I databse/mysql
# Settings for .so output
CFLAGS+= `$(MYSQLCFG) --cflags` -shared -ggdb
LIBS+= -L/usr/lib/mysql -lmysqlclient -lcrypt -lnsl -lm -lc 
#-lnss_files -lnss_dns -lresolv -lc -lnss_files -lnss_dns -lresolv



all:
	$(CXX) $(CFLAGS) -I$(INCDIRS)  $(SRCS) $(LIBS) -o $(OUTNAME).so

nolink:
	$(CXX) $(CFLAGS) -I$(INCDIRS) $(SRCS) -c

clean:
	rm *.so *.o
