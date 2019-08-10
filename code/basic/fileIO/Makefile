# Author: Hang Yuan (hyuan211@gmail.com)
GPPWARN     = -Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast
GPPOPTS     = ${GPPWARN} -fdiagnostics-color=never
COMPILECPP  = g++ -std=gnu++2a -g -O0 ${GPPOPTS}

MODULES   = fileio
EXECBINS  = unit_test
CPPHEADER = ${MODULES:=.h} ${EXECBINS:=.h}
CPPSOURCE = $(MODULES:=.cpp) $(EXECBINS:=.cpp)
OBJECTS   = $(CPPSOURCE:.cpp=.o)
CLEANOBJS = $(OBJECTS) $(EXECBINS)

$(EXECBINS): $(OBJECTS)
	$(COMPILECPP) -o $@ $(OBJECTS)

%.o: %.cpp
	$(COMPILECPP) -c $<

.PHONY: clean
clean:
	- rm $(OBJECTS)

.PHONY: cleanall
cleanall:
	-rm $(CLEANOBJS) *.log
