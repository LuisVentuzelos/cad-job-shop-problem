# My first makefile

openmp: 
	gcc-11 -Wall -fopenmp job-shop.c -o job-shop

normal: 
	gcc-11 -Wall job-shop.c -o job-shop

noWarnings:
	gcc-11 job-shop.c -o job-shop

clean:
	rm -rf *.o *~ job-shop

