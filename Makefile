CC=gcc

CFLAGS=
INCLUDE=
LIBPATH=
LIBS= -lpthread

SRCS = ads1015drvr.c
OBJS = $(SRCS:.c=.o)

.PHONY: libads1015drvr.a $(OBJS)

all: libads1015drvr.a

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBPATH) $(LIBS) -c -o $@ $^

libads1015drvr.a: $(OBJS)
	ar -cvq libads1015drvr.a $(OBJS)

clean:
	rm *.o *.a