make
fn=("DB")
# fn=("DB" "LJ" "CP")
# set only store sim >= 0.001
for i in ${fn[*]}; do
    ./EXSim -d ../dataset/$i.txt -f $i -algo ExactSim -e 1.0e-06 -qn 100
done