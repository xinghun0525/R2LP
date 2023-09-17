make gene_topk_large_graph

fn=("CG" "WV" "EN" "SD" "WF" "DB" "ND" "CP" "LJ" "WZ")
for((i=7;i<10;i++)); do
    echo "${fn[$i]} $j start!!"
    ./gene_topk_large_graph ${fn[$i]} $j
    echo "${fn[$i]} $j finished!!"
done
