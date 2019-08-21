# http://www.360doc.com/content/17/0717/10/8335678_672019543.shtml

#CC = gcc
#CFLAGS = -g -Wall
CC = g++
CXXFLAGS = -std=c++11 -g -Wall

# ippcp.h
INCLUDE_DIR = /opt/intel/ipp/compilers_and_libraries_2019.4.243/linux/ippcp/include

# libippcp.so
LIB = ippcp
LIBRARY_PATH= /opt/intel/ipp/compilers_and_libraries_2019.4.243/linux/ippcp/lib/intel64_lin/
#LD_FLAGS = -L$(LIBRARY_PATH) -l$(LIB) -Wl,-rpath=$(LIBRARY_PATH)
LD_FLAGS = -Wl,-rpath=$(LIBRARY_PATH) -L$(LIBRARY_PATH) -l$(LIB)

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

EXEC = xydemo
start: $(OBJS)
	$(CC) $(CXXFLAGS) -I$(INCLUDE_DIR) $(LD_FLAGS) -o $(EXEC) $(OBJS)

.cpp.o:
	$(CC) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ -c $<

.PHONY: clean
clean:
	rm -fr $(EXEC) $(OBJS)

install:
	cp $(EXEC) /tmp/
uninstall:
	rm /tmp/$(EXEC)
