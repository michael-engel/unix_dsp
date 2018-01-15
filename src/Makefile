SOURCES := $(wildcard *.c)
OBJECTS := ${SOURCES:.c=.o}
EXECUTABLE := executable
CC := gcc

all: clean ${EXECUTABLE}
	./${EXECUTABLE}

${EXECUTABLE}: ${OBJECTS}
	${CXX} $^ -o $@

#pattern matching target and dependencies
%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f ${EXECUTABLE} ${OBJECTS}

help:
	@echo "all:\n   Builds executable\nclean:\n   Deletes binary files"
