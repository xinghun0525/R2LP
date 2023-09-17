cd ../../code
make main_eps_large_graph
fn=("CP" "LJ" "WZ")
eps=("0.1" "0.05" "0.01")
for i in ${fn[*]}; do
    mkdir -p ../log/eps_query/$i
    for k in ${eps[*]}; do
        echo "$i $k start!!"
        ./main_eps_large_graph $i $k 2>&1 > ../log/eps_query/$i/eps-$k.log
        echo "$i $k finished!!"
    done
done