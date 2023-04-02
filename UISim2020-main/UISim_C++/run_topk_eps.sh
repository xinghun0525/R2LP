nodeFilelist=("/UISim2020-main/data/CG/nodes.txt" "/UISim2020-main/data/WV/nodes.txt" "/UISim2020-main/data/EN/nodes.txt" "/UISim2020-main/data/SD/nodes.txt" "/UISim2020-main/data/WF/nodes.txt" "/UISim2020-main/data/DB/nodes.txt" "/UISim2020-main/data/ND/nodes.txt" "/UISim2020-main/data/CP/nodes.txt" "/UISim2020-main/data/LJ/nodes.txt" "/UISim2020-main/data/WZ/nodes.txt")
edgeFilelist=("/UISim2020-main/data/CG/edges.txt" "/UISim2020-main/data/WV/edges.txt" "/UISim2020-main/data/EN/edges.txt" "/UISim2020-main/data/SD/edges.txt" "/UISim2020-main/data/WF/edges.txt" "/UISim2020-main/data/DB/edges.txt" "/UISim2020-main/data/ND/edges.txt" "/UISim2020-main/data/CP/edges.txt" "/UISim2020-main/data/LJ/edges.txt" "/UISim2020-main/data/WZ/edges.txt")
indexDirlist=("/UISim2020-main/data/CG/" "/UISim2020-main/data/WV/" "/UISim2020-main/data/EN/" "/UISim2020-main/data/SD/" "/UISim2020-main/data/WF/" "/UISim2020-main/data/DB/" "/UISim2020-main/data/ND/" "/UISim2020-main/data/CP/" "/UISim2020-main/data/LJ/" "/UISim2020-main/data/WZ/")
outputDir=("/UISim2020-main/result/CG/" "/UISim2020-main/result/WV/" "/UISim2020-main/result/EN/" "/UISim2020-main/result/SD/" "/UISim2020-main/result/WF/" "/UISim2020-main/result/DB/" "/UISim2020-main/result/ND/" "/UISim2020-main/result/CP/" "/UISim2020-main/result/LJ/" "/UISim2020-main/result/WZ/")

# hubsNum=("20% 50%", ...)
HubNum=("1048 2620" "1423 3557" "7338 18345" "16433 41082" "13112 32780" "63416 158540" "65145 162862" "754953 1887384" "969514 2423785" "357276 893190")

stopRea=("0.0001 0.001" "0.0001 0.001" "0.1 0.01 0.001" "0.1 0.05 0.01" "0.1 0.05 0.01" "0.1 0.05 0.01" "0.1 0.05 0.01" "0.1 0.05 0.01" "0.1 0.05 0.01" "0.1 0.05 0.01")

hubtype="indeg"
depth="5"


eta="2"
make
cd release
# ./SelectHubs indeg


for((i=0;i<2;i++));do
    for e in ${eta};do
        for sr in ${stopRea[$i]};do
            for hn in ${HubNum[$i]};do
                ./Online_ap $hubtype $hn $depth $sr $e ${nodeFilelist[$i]} ${edgeFilelist[$i]} ${indexDirlist[$i]} ${outputDir[$i]}
            done
        done
    done
done

