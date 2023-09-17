cd Local-Push/build
cmake .. -DBUILD_PLAYGROUND=OFF -DEXTRA_INCLUDE_DIR=../../dependencies/include/Eigen -DDATA_ROOT_PATH=../../dataset
make -j
fn=("CG" "WV" "EN" "SD" "DB" "WF" "ND")
eps=1e-6
save_sim=1e-3
for i in ${fn[*]}; do
    echo "$i start!!"
    ./get_gtsim $i $eps $save_sim
    echo "$i finished!!"
done


