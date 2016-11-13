SOURCES := main.cc
OBJS := $(SOURCES:.cc=.o)

all: styrene

styrene: $(OBJS)
	$(CC) $(CFLAGS) -o styrene $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<
