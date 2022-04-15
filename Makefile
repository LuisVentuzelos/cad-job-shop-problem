# My first makefile

threads: 
	gcc-11 -Wall -fopenmp -lpthread job-shop-threads.c -o job-shop-threads


normal: 
	gcc-11 -Wall job-shop.c -o job-shop

noWarnings:
	gcc-11 job-shop.c -o job-shop

clean:
	rm -rf *.o *~ job-shop

