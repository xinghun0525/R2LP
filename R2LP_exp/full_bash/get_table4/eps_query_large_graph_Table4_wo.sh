cd ../../code
rm main_eps_large_graph_wo_pruning
make main_eps_large_graph_wo_pruning
fn=("CP LJ wiki_zh")
#fn=("LJ")
eps=("0.01")
for i in ${fn[*]}; do
    mkdir -p ../rbslog/eps_query/$i
    for k in ${eps[*]}; do
        echo "$i $k start!!"
        ./main_eps_large_graph_wo_pruning $i $k 2>&1 > ../rbslog/eps_query/$i/eps-$k.log
        echo "$i $k finished!!"
    done
done
