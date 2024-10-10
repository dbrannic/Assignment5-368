WARNING = -Wall -Wshadow --pedantic 
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)

SRCS = a5.c
OBJS = $(SRCS:%.c=%.o)

a5: $(OBJS)
		$(GCC) $(OBJS) -o a5

.c.o:
		$(GCC) -c $*.c 

clean:
	rm -f a5 *.o output? *~
