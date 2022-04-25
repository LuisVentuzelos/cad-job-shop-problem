#Sequencial
for ((i=1; i<=6; i=i+1))
do 
      echo "Executing Test $i"
      
      for ((j=1; j<=10; j=j+1))
      do 
            ./job-shop data/input/input0$i data/output/output0$i
      done
done
