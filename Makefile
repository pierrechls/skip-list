EXECUTABLE = skip-list
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(EXECUTABLE)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(EXECUTABLE) $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@
	@echo "------------------------------------------------------------"
	@echo "    to execute without file   : ./$(EXECUTABLE)"
	@echo "    to execute with text file : ./$(EXECUTABLE) file.txt"
	@echo "    to execute with csv file  : ./$(EXECUTABLE) file.csv"
	@echo "------------------------------------------------------------"

clean:
	@echo "------------------------------------------------------------"
	@echo "                       CLEAN
	@echo "------------------------------------------------------------"
	-rm -f *.o
	-rm -f $(EXECUTABLE)
