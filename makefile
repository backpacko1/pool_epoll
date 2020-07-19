
src = $(wildcard *.c)
obj = $(patsubst %.c, %.o, $(src))

server:$(obj)
	gcc $(^) -o $(@) -lpthread -lsqlite3
server.o:server.c
	gcc -c $(<) -o $(@)
epoll.o:epoll.c
	gcc -c $(<) -o $(@) 
sock.o:sock.c
	gcc -c $(<) -o $(@)
pool.o:pool.c
	gcc -c $(<) -o $(@) 
function.o:function.c
	gcc -c $(<) -o $(@)
	
.PHONY:clean
clean:
	-rm server $(obj)
