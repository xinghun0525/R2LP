cd ../../code
make main_eps_small_graph
fn=("CG" "WV" "EN")
eps=("0.05" "0.01" "0.005" "0.001")
for i in ${fn[*]}; do
    mkdir -p ../log/eps_query/$i
    for k in ${eps[*]}; do
        echo "$i $k start!!"
        ./main_eps_small_graph $i $k 2>&1 > ../log/eps_query/$i/eps-$k.log
        echo "$i $k finished!!"
    done
done