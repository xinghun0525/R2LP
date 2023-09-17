make gene_topk_small_graph

fn=("CG" "WV" "EN" "SD" "WF" "DB" "ND" "CP" "LJ" "WZ")
for((i=0;i<6;i++)); do
    echo "${fn[$i]} start!!"
    ./gene_topk_small_graph ${fn[$i]}
    echo "${fn[$i]} finished!!"
done
