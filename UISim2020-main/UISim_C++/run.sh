# 
nodeFilelist=("../../data/CG/nodes.txt" "../../data/WV/nodes.txt" "../../data/EN/nodes.txt" "../../data/SD/nodes.txt" "../../data/WF/nodes.txt" "../../data/DB/nodes.txt" "../../data/ND/nodes.txt" "../../data/CP/nodes.txt" "../../data/LJ/nodes.txt" "../../data/WZ/nodes.txt")
edgeFilelist=("../../data/CG/edges.txt" "../../data/WV/edges.txt" "../../data/EN/edges.txt" "../../data/SD/edges.txt" "../../data/WF/edges.txt" "../../data/DB/edges.txt" "../../data/ND/edges.txt" "../../data/CP/edges.txt" "../../data/LJ/edges.txt" "../../data/WZ/edges.txt")
indexDirlist=("../../data/CG/" "../../data/WV/" "../../data/EN/" "../../data/SD/" "../../data/WF/" "../../data/DB/" "../../data/ND/" "../../data/CP/" "../../data/LJ/" "../../data/WZ/")
outputDir=("../../result/CG/" "../../result/WV/" "../../result/EN/" "../../result/SD/" "../../result/WF/" "../../result/DB/" "../../result/ND/" "../../result/CP/" "../../result/LJ/" "../../result/WZ/")
HubNum=("1048 2620" "1423 3557" "7338 18345" "16433 41082" "13112 32780" "63416 158540" "65145 162862" "754953 1887384" "969514 2423785" "357276 893190")

hubtype="indeg"
depth="5"
eta="2"
make
cd release

# To select the hubs node, you only need to run it once, and the results are saved in 'data/{graph_name}/indeg'.
# ./SelectHubs indeg

stopRea="0.005 0.001"  # small graph
for((i=0;i<2;i++));do
    for e in ${eta};do
        for sr in ${stopRea};do
            for hn in ${HubNum[$i]};do
                ./Online_ap $hubtype $hn $depth $sr $e ${nodeFilelist[$i]} ${edgeFilelist[$i]} ${indexDirlist[$i]} ${outputDir[$i]}
            done
        done
    done
done

stopRea="0.05 0.01"   # middle graph
for((i=2;i<7;i++));
do
    for hn in ${HubNum[$i]}
    do
        for sr in ${stopRea};do
            for hn in ${HubNum[$i]};do
                ./Online_ap $hubtype $hn $depth $sr $e ${nodeFilelist[$i]} ${edgeFilelist[$i]} ${indexDirlist[$i]} ${outputDir[$i]}
            done
        done
    done
done

stopRea="0.01"  # large graph
for((i=7;i<10;i++));
do
    for hn in ${HubNum[$i]}
    do
        for sr in ${stopRea};do
            for hn in ${HubNum[$i]};do
                ./Online_ap $hubtype $hn $depth $sr $e ${nodeFilelist[$i]} ${edgeFilelist[$i]} ${indexDirlist[$i]} ${outputDir[$i]}
            done
        done
    done
done