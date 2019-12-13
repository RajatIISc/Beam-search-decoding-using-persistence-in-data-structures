a.out: client.o impl.o
	gcc client.o impl.o
client.o: client.c header.h
	gcc -c client.c
impl.o: impl.c header.h
	gcc -c impl.c
