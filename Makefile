# My first makefile

threads: 
	gcc-11 -Wall -fopenmp -lpthread job-shop-threads-operations.c -o job-shop-threads-operations
	gcc-11 -Wall -fopenmp -lpthread job-shop-threads-jobs.c -o job-shop-threads-jobs

normal: 
	gcc-11 -Wall job-shop.c -o job-shop

noWarnings:
	gcc-11 job-shop.c -o job-shop

clean:
	rm -rf *.o *~ job-shop
	rm -rf *.o *~ job-shop-threads-operations
	rm -rf *.o *~ job-shop-threads-jobs
