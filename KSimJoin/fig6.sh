g++ -march=core2 -pthread -std=c++11 -DSFMT_MEXP=607 -I SFMT-src-1.4.1 -O3 -o KSimJoin_eps SFMT-src-1.4.1/SFMT.c main_eps_query.cpp
fn=("CG" "WV")
max_lengths=("2")
times=("100000")
e=("0.01")
for t in ${times[*]}; do 
    for f in ${fn[*]}; do 
        mkdir -p ./log/eps_query/$f/
        mkdir -p ./result/$f/
        for max_length in ${max_lengths[*]}; do 
            echo "KSimJoin_eps $f times-$t eps-$e max_length-$max_length start!!"
            ./KSimJoin_eps $f $t $max_length $e  2>&1 > ./log/eps_query/$f/times$t-l$max_length-eps$e.log
            echo "KSimJoin_eps $f times-$t eps-$e max_length-$max_length finished!!"
        done
    done
done


