IDIR = ./include
SOURCEDIR = ./src
LIB = ncurses

CC = gcc
CFLAGS = -g -I $(IDIR) $(SRC) -l $(LIB)

SOURCES = $(wildcard $(SOURCEDIR)/*.c)

all: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)
