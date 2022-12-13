EXEC = ants
PROJECT_NAME = ants_game

EXEC_O = $(EXEC:=.o)
OBJETS_O = $(OBJETS) $(EXEC_O)

CC = gcc
CCFLAGS_STD = -Wall -O3 -Werror
CCFLAGS_DEBUG = -D _DEBUG_
CCFLAGS = $(CCFLAGS_STD)
CCLIBS = -lncursesw -lpthread

all: msg $(OBJETS) $(EXEC_O)
	@echo "Executable creation..."
	@for i in $(EXEC); do \
	$(CC) -o $$i $$i.o $(OBJETS) $(CCLIBS); \
	done
	@echo "Done."

msg:
	@echo "Objects creation..."

clean:
	@echo "Cleaning..."
	@rm -f $(OBJETS) $(EXEC_O)
	@rm -f *~ *#
	@rm -f $(EXEC)
	@echo "Done."

ants.o: ants.c 