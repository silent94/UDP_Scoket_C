CROSS_COMPILE =
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
CFLAGS = -fPIC -W -Wno-unused-result -O3 -D_REENTRANT -DDEBUG -g

INCLUDE =

LIBS =
LIBS += -lev

obj_1 = sender.o 
obj_2 = receiver.o

all: clean main_1 main_2 romfs fclean

main_1: $(obj_1)
	$(CC) $(CFLAGS) -o sender $(obj_1) $(LIBS) $(INCLUDE)

main_2: $(obj_2)
	$(CC) $(CFLAGS) -o receiver $(obj_2) $(LIBS) $(INCLUDE)

clean:
	rm -f *.o sender receiver
	@echo --- all references deleted

romfs:
	@echo --- make done
%.o:%.c
	gcc -c $< $(INCLUDE)

fclean:
	rm -f *.o
	@echo --- all references deleted