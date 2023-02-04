main: main.c parse.o parse.h avl_tree.o avl_tree.h exec_job.o exec_job.h
	gcc -o main main.c parse.o avl_tree.o exec_job.o
parse.o: parse.c parse.h process.h
	gcc -c parse.c
exec_job.o: exec_job.c exec_job.h process.h avl_tree.h
	gcc -c exec_job.c
process.o: process.c process.h
	gcc -c process.c
avl_tree.o: avl_tree.c avl_tree.h
	gcc -c avl_tree.c
clean:
	rm -f main parse.o process.o avl_tree.o
