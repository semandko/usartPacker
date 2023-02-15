TARGET = prog
LIBS = -lm
CC = g++
CFLAGS = -g -Wall
 
.PHONY: clean all default
 
default: $(TARGET)
all: default
 
OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)
 
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
	
.PRECIOUS: $(TARGET) $(OBJECTS)
 
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@
	
clean:
	del *.o
	del $(TARGET)