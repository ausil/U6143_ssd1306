Target		= display
CXX         	= gcc
CFLAGS		= -Wall -g
ODIR		= obj
SRC		=$(wildcard *.c)
DIR		=$(notdir $(SRC))
OBJ 		= $(patsubst %.c, %.o, $(DIR))

$(Target): $(OBJ)
	$(CXX) $(CFLAGS) $^ -o $@ -rdynamic

%.o: %.c
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -rf *.o
	rm -rf $(Target)
