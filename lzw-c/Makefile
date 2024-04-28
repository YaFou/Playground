lzw.o: lzw.c lzw.h
	gcc -c lzw.c

main.o: main.c lzw.h
	gcc -c main.c

main: main.o lzw.o
	gcc -o main main.o lzw.o

test: main
	./main C test/t1.txt test/t1.lzw
	./main D test/t1.lzw test/t1d.txt
	diff test/t1.txt test/t1d.txt

	./main C test/t2.txt test/t2.lzw
	./main D test/t2.lzw test/t2d.txt
	diff test/t2.txt test/t2d.txt

	./main c test/t1.txt test/t1.lzw
	./main d test/t1.lzw test/t1d.txt
	diff test/t1.txt test/t1d.txt

	./main c test/t2.txt test/t2.lzw
	./main d test/t2.lzw test/t2d.txt
	diff test/t2.txt test/t2d.txt