rm gene_topk_medium_graph
g++ -std=c++11 gene_topk_medium_graph.cpp -o gene_topk_medium_graph
fn=("EN")
#fn=("DB")
#fn=("SD DB wiki_fy ND")
for i in ${fn[*]}; do
    mkdir -p ./mediumlog
    echo "$i start!!"
    ./gene_topk_medium_graph $i 2>&1 > ./mediumlog/$i.log
    echo "$i finished!!"
done