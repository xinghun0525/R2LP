cd Local-Push/build
cmake .. -DBUILD_PLAYGROUND=OFF -DEXTRA_INCLUDE_DIR=../../dependencies/include/Eigen -DDATA_ROOT_PATH=../../dataset
make -j
# fn=("SD" "DB" "WF" "ND")
fn=("WZ")
# eps=("0.05" "0.01" "0.005" "0.001")
# fn=("CP" "LJ" "WZ")

eps=("0.1" "0.05" "0.01")
for i in ${fn[*]}; do
    for k in ${eps[*]}; do
        mkdir -p ../log/eps_query/$i
        echo "$i $k start!!"
        ./rlp $i $k 2>&1 > ../log/eps_query/$i/eps_$k.log
        echo "$i $k finished!!"
    done
done


