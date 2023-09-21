rm gt_sort
g++ -std=c++11 gt_sort.cpp -o gt_sort
fn=("CG WV")
#fn=("EN SD DB wiki_fy ND")
for i in ${fn[*]}; do
    echo "$i start!!"
    ./gt_sort $i
    echo "$i finished!!"
done