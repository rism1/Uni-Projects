CC=g++
CFLAGS=-c -pedantic-errors
LDFLAGS=
SOURCES=boolcalc.cpp boolexpr.cpp calc.cpp formulae.cpp formulaenode.cpp functions.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=boolcalc

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)