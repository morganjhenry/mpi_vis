
MPICC=mpicc
CC=gcc

#all: orig hybrid recv postcopy mvapich
all: orig 

orig:
	${MPICC} -o send_recv send_recv.c -Xlinker -rpath ${PWD}/../lib \
		-L${PWD}/../lib \
		-lmyib_c -O3 -libverbs -DUSE_MYIB
	
hybrid: 
	${MPICC} -g -o send_recv_hybrid send_recv.c ../lib/libhybrid_c.a -D USE_HYBRID -libverbs

postcopy: 
	${MPICC} -g -o send_recv_postcopy send_recv.c ../lib/libsend_init_c.a -libverbs

mvapich: 
	${MPICC} -g -o send_recv_mvapich send_recv.c ../lib/libmvapich_c.a -libverbs

recv: 
	${MPICC} -g -o send_recv_recv send_recv.c ../lib/librecv_init_c.a -libverbs

clean:
	/bin/rm send_recv send_recv_hybrid send_recv_postcopy send_recv_mvapich send_recv_recv -f
