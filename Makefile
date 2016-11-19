SOURCES := main.cc console.cc
OBJS := $(SOURCES:.cc=.o)
CC := c++
CFLAGS := -g -Wall

all: styrene

clean:
	rm -f $(OBJS)
	rm -f styrene

styrene: $(OBJS)
	$(CC) $(CFLAGS) -o styrene $(OBJS) $(LFLAGS) $(LIBS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<
