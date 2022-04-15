# cad-job-shop-problem

Simple Job Shop Problem C Algorithm for Computação de Alto Desempenho discipline of IPCA (Polytechnic Institute of Cávado and Ave)

## Autores

- [@Luis Ventuzelos](https://www.github.com/LuisVentuzelos)
- [@Ricardo Faria](https://www.github.com/RicardoMSFaria)
- [@João Cunha](https://www.github.com/JCunha99)

## Compile

To compile the Application run the following command

```bash
  make normal
```

To compile the Application with threads run the following command

```bash
  make threads
```

## Run the Application without threads

To run the Application run the following command

```bash
   ./job-shop data/input01 output/output01
```

## Run the Application with threads

To run the Application run the following command

```bash
   ./job-shop-threads data/input01 5 output/output01
```

## Appendix

### Fisher and Thompson 3x3 instance example for the job shop problem

#1 - Number of Machines

#2 - Number of Jobs

#3 - Number of Operations per Job

#4 - Matrix of processing times and machine allocation

```bash
3
3
3
0 3 1 2 2 2
0 2 2 1 1 4
1 4 2 3 0 0

```
