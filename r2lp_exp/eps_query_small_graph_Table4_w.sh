cd code
rm  main_eps_small_graph_w_pruning
make main_eps_small_graph_w_pruning
fn=("CG WV EN SD DB wiki_fy ND")
eps=("0.01")
for i in ${fn[*]}; do
    mkdir -p ../rbslog/eps_query/$i
    for k in ${eps[*]}; do
        echo "$i $k start!!"
        ./main_eps_small_graph_w_pruning $i $k 2>&1 > ../rbslog/eps_query/$i/eps-$k.log
        echo "$i $k finished!!"
    done
done