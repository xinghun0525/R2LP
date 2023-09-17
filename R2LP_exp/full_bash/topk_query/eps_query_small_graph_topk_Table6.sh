cd ../../code
rm main_small_graph_topk
make main_small_graph_topk
fn=("CG WV")
#fn=("EN SD DB wiki_fy ND")
topk=("500 1000 5000 10000")
eps=("0.000001")
for i in ${fn[*]}; do
    mkdir -p ../rbslog/eps_query/$i
    for k in ${eps[*]}; do
      for j in ${topk[*]}; do
        echo "$i $k tok $j start!!"
        ./main_small_graph_topk $i $j $k 2>&1 > ../rbslog/eps_query/$i/eps-$k-top$j.log
        echo "$i $k tok $j finished!!"
        done
    done

done
