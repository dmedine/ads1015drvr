CC=gcc

NAME?=check_macros

CFLAGS=
INCLUDE=-I../
LIBPATH=-L/home/pi/piI2C
LIBS=-lads1015drvr -lpthread

SRCS = $(NAME.c)
OBJS = $(SRCS:.c=.o)

.PHONY: $(NAME)

$(NAME): $(NAME.c)
	$(CC) -o $(NAME) $(NAME).c $(CFLAGS) $(INCLUDE) $(LIBPATH) $(LIBS)

clean:
	rm $(NAME)