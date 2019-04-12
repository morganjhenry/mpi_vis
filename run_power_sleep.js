#!/bin/bash

#SBATCH --nodes=2
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=morganhenry@email.arizona.edu
#SBATCH --time=05:59:00

nnodes=2
nranks=36
niters=100
fname="pcap100_sleep_1.js"

echo "data = [ " > $fname
for pcap in 50 70 90;
do
    srun -N ${nnodes} -n ${nnodes}  --ntasks-per-node=1 /g/g20/henry37/myib/scripts/set_power ${pcap} 
    for buffsize in 1000 2000 5000 10000 20000 50000 550000 1050000;
    do
        for ssleep in 10 50 100 500;
        do
            for rsleep in 10 50 100 500;
            do
                for proto in postcopy mvapich recv hybrid;
                do
                    time1=`MYIB_EAGER_THRESHOLD=10000 MYIB_PROTOCOL=${proto} \
                              srun -N ${nnodes} -n ${nranks} --ntasks-per-node=18 \
                              ./send_recv ${buffsize} ${niters} ${ssleep} ${rsleep} \
                              | grep "Total Exec" | cut -d ":" -f 2 | sort -r | head -n 1`
                    echo { \"buffsize\": $buffsize, \"niters\": $niters, \"ssleep\": $ssleep, \"rsleep\": $rsleep, \"nnodes\": $nnodes, \"nranks\": $nranks, \"proto\": \"$proto\", \"pcap\": $pcap, \"time\" : $time1 }, >> $fname
                done
            done
        done        
    done    
done
echo "]; " >> $fname
