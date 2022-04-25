#Thread Jobs
echo "Executing Job Shop Jobs Parrallized"
for ((i=1; i<=6; i=i+1))
do 
      echo "Executing Test $i"
      
      for ((j=1; j<=10; j=j+1))
      do 
            ./job-shop-threads-jobs data/input/input0$i data/output/output0$i 5
      done
done


#Threads Operations
echo "Executing Job Shop Operations Parrallized"
for ((i=1; i<=6; i=i+1))
do 
      echo "Executing Test $i"
      
      for ((j=1; j<=10; j=j+1))
      do 
            ./job-shop-threads-operations data/input/input0$i data/output/output0$i 5
      done
done
