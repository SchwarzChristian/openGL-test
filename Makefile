CC = g++
CFLAGS = -Wall -DGL_API_EXPERIMENTAL
LIBRARIES = -lGL -lGLU -lglut -lpng -lm
OBJECTS = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))

all: $(OBJECTS)
	$(CC) -o openGLtest $(LIBRARIES) $(OBJECTS)

%.o: %.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f ./openGLtest
	rm -f src/*.o
	rm -f *~
