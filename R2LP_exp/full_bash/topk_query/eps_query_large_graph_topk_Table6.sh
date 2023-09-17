cd code
rm main_large_graph_topk
g++ -march=core2  -std=c++11 -DSFMT_MEXP=607 -I SFMT-src-1.4.1/ -O3 -o main_large_graph_topk main_large_graph_topk.cpp SFMT.c
fn=("WZ")
#fn=("CP LJ WZ")
topk=("500 1000 5000 10000")
eps=("0.000001")
for i in ${fn[*]}; do
    mkdir -p ../rbslog/eps_query/$i
    for k in ${eps[*]}; do
      for j in ${topk[*]}; do
        echo "$i $k tok $j start!!"
        ./main_large_graph_topk $i $j $k 2>&1 > ../rbslog/eps_query/$i/eps-$k-top$j.log
        echo "$i $k tok $j finished!!"
        done
    done

done