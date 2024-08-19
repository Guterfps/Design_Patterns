CC = g++
CFLAGS = -fPIC -std=c++11 -pedantic-errors -Wall -Wextra
CCFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra
LDFLAGS = -shared 
TARGET = $(shell basename $(CURDIR))
DIRLIB = $(shell dirname $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST)))))/libs/$(MAKECMDGOALS)
DIROBJ = $(shell dirname $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST)))))/obj/$(MAKECMDGOALS)
TARGET_LIB = lib$(TARGET).so
DIRINC = $(shell dirname $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST)))))/include/
DEPLIBS = semaphore 
LINKFLAGS = $(addprefix -l,$(DEPLIBS))
INCLIBS = pq semaphore
LINKHADEARS = $(addprefix -I,$(INCLIBS))


define newline


endef

.PHONY: all clean debug release

all: 
	make debug -s
	make release -s

debug: CFLAGS += -g
debug: test
			
release: CFLAGS += -DNDEBUG -O3
release: test_release

test: DIRLIB = ../libs/debug
test: CCFLAGS += -g
test: $(TARGET).out

test_release: DIRLIB = ../libs/release
test_release: CCFLAGS += -DNDEBUG -O3
test_release: $(TARGET).out
		
$(TARGET).out: $(TARGET)_test.o $(DIRINC)
		$(foreach dir,$(DEPLIBS),@$(MAKE) $(MAKECMDGOALS) -C ../$(dir) $(newline))
		$(CC) $(CCFLAGS) $(TARGET)_test.o -o $(TARGET).out -L$(DIRLIB) \
		-Wl,-rpath,$(DIRLIB) $(LINKFLAGS)
		
$(TARGET): $(TARGET).o $(TARGET)_test.o $(DIRINC)
		$(CC) $(CFLAGS) -o $(TARGET).out $(TARGET).o $(TARGET)_test.o
		

$(TARGET)_test.o: $(TARGET)_test.cpp $(DIRINC)
		$(CC) $(CCFLAGS) -I$(DIRINC) -c $(TARGET)_test.cpp $(LINKHADEARS)


clean:
		$(RM) *.out *.o
		$(foreach dir,$(DEPLIBS),@$(MAKE) $(MAKECMDGOALS) -C ../$(dir) $(newline))
