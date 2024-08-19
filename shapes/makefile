CC = g++
CFLAGS = -fPIC -std=c++98 -pedantic-errors -Wall -Wextra
CCFLAGS = -std=c++98 -pedantic-errors -Wall -Wextra
LDFLAGS = -shared 
TARGET = $(shell basename $(CURDIR))
DIRLIB = $(shell dirname $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST)))))/libs/$(MAKECMDGOALS)
DIROBJ = $(shell dirname $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST)))))/obj/$(MAKECMDGOALS)
TARGET_LIB = lib$(TARGET).so
DIRINC = $(shell dirname $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST)))))/include/
DEPLIBS = 
LINKFLAGS = $(addprefix -l,$(DEPLIBS))

SRCS = $(TARGET).cpp
OBJS = $(SRCS:.cpp=.o)

define newline


endef

.PHONY: all clean debug release

all: 
	make debug -s
	make release -s

debug: CFLAGS += -g
debug: lib
			
release: CFLAGS += -DNDEBUG -O3
release: lib

lib: $(DIRLIB)/${TARGET_LIB}

test: DIRLIB = ../libs/debug
test: CCFLAGS += -g
test: $(TARGET).out

test_release: DIRLIB = ../libs/release
test_release: CCFLAGS += -DNDEBUG -O3
test_release: $(TARGET).out
		
$(TARGET).out: $(TARGET)_test.o 
		$(CC) $(CCFLAGS) $(TARGET)_test.o -o $(TARGET).out -I$(DIRINC) -L$(DIRLIB) \
		-Wl,-rpath,$(DIRLIB) -l$(TARGET) 
		
$(TARGET): $(TARGET).o $(TARGET)_test.o
		$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).o $(TARGET)_test.o
		
$(DIROBJ)/$(TARGET).o: $(TARGET).cpp $(DIRINC)
		$(CC) $(CFLAGS) -c $(TARGET).cpp -I$(DIRINC) -o $(DIROBJ)/$(TARGET).o 

$(TARGET)_test.o: $(TARGET)_test.cpp $(DIRINC)
		$(CC) $(CCFLAGS) -c $(TARGET)_test.cpp -I$(DIRINC)

$(DIRLIB)/$(TARGET_LIB): $(DIROBJ)/$(OBJS) $(DIRINC)/$(TARGET).hpp
		$(foreach dir,$(DEPLIBS),@$(MAKE) $(MAKECMDGOALS) -C ../$(dir) $(newline))
		$(CC) $(LDFLAGS) -o $@ $^ -I$(DIRINC) -L$(DIRLIB) \
		-Wl,-rpath,$(DIRLIB) $(LINKFLAGS)


$(SRCS:.cpp=.d):%.d:%.cpp
		$(CC) $(CFLAGS) -I$(DIRINC) -MM $< >$@

include $(SRCS:.cpp=.d)

clean:
		$(RM) ../libs/debug/${TARGET_LIB} ../libs/release/${TARGET_LIB}\
		../obj/debug/${OBJS} $(SRCS:.cpp=.d) ../obj/release/${OBJS} *.out *.o
		$(foreach dir,$(DEPLIBS),@$(MAKE) $(MAKECMDGOALS) -C ../$(dir) $(newline))

