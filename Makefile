SOURCES := main.cc console.cc board.cc
OBJS := $(SOURCES:.cc=.o)
DEPS = $(OBJS:%.o=%.d)

CC := c++
CFLAGS := -g -Wall

all: styrene

.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)
	rm -f styrene

styrene: $(OBJS)
	$(CC) $(CFLAGS) -o styrene $(OBJS) $(LFLAGS) $(LIBS)

-include $(DEPS)

.cc.o:
	$(CC) $(CFLAGS) -MMD -c $<
