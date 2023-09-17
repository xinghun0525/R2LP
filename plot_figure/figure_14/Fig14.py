import matplotlib.pyplot as plt
import time
import os
from matplotlib.path import Path
import matplotlib.patches as patches


small_graph = ["CG", "WV", "EN"]
middle_graph = ["SD", "DB", "WF", "ND"]
large_graph = ["CP", "LJ", "WZ"]

R2LP = {
"CG": [
    [1,0.999757,1,1],[0.050184,0.200651,0.406046,1.66729],
    [0.999935,1,1,0.999985],[0.050184,0.200651,0.406046,1.66729],
    [0.999272,0.996142,0.999531,1],[0.050184,0.200651,0.406046,1.66729]
],
"WV": [
    [1,1,1,1],[1.00507,1.13108,1.32654,4.2667],
[0.999659,0.99983,0.99955,0.999178],[1.00507,1.13108,1.32654,4.2667],
[0.93964,0.999938,0.999862,0.999823],[1.00507,1.13108,1.32654,4.2667]
],
"EN": [
    [0.999991,0.999999,0.999999,1],[7.99674,12.8597,20.5388,132.234],
[0.999947,0.999991,0.999996,1],[7.99674,12.8597,20.5388,132.234],
[0.999235,0.998424,0.999548,0.999999],[7.99674,12.8597,20.5388,132.234]
],
"SD": [
    [0.999996,0.999999,1,1],[22.5085,37.0945,63.6205,452.957],
[0.999523,0.999883,0.999936,0.999999],[22.5085,37.0945,63.6205,452.957],
[0.992497,0.978409,0.993494,0.999979],[22.5085,37.0945,63.6205,452.957]
],
"DB": [
    [0.99995,0.999916,0.999905,1],[9.31714,41.0136,75.1262,328.059],
[0.999845,0.999956,0.999976,0.999997],[9.31714,41.0136,75.1262,328.059],
[0.998956,0.992584,0.99814,0.999972],[9.31714,41.0136,75.1262,328.059]
],
"WF": [
    [0.999952,1,1,1],[9.62989,38.5576,78.421,399.126],
[0.996337,0.999728,0.999906,0.999993],[9.62989,38.5576,78.421,399.126],
[0.971038,0.9852,0.996989,0.999953],[9.62989,38.5576,78.421,399.126]
],
"ND": [
    [0.999834,0.999998,1,1],[429.713,2567.6,3767.71,8218.34],
[0.998131,0.999879,0.999988,1],[429.713,2567.6,3767.71,8218.34],
[0.999321,0.999019,0.999682,0.999984],[429.713,2567.6,3767.71,8218.34]
],

"CP": [
    [1,1,1],[86.8034,122.792,481.532],
[0.949441,0.980537,0.994651],[86.8034,122.792,481.532],
[0.951623,0.896499,0.934814],[86.8034,122.792,481.532]
],
"LJ": [
    [1,1,1],[670.386,1382.12,3070.25],
[0.998903,0.999566,0.998752],[670.386,1382.12,3070.25],
[0.848331,0.992558,0.98487],[670.386,1382.12,3070.25]
],
"WZ": [
    [0.999891,1,1],[538.561,849.529,3490.99],
[0.997217,0.998232,0.999742],[538.561,849.529,3490.99],
[0.86105,0.97514,0.984876],[538.561,849.529,3490.99]
],
}
OptLP = {
"CG": [
    [1,1,1,1],[0.173553,1.74211,2.79523,8.26265],
    [1,1,1,1],[0.173553,1.74211,2.79523,8.26265],
    [1,1,1,1],[0.173553,1.74211,2.79523,8.26265]
],
"WV": [
    [1,1,1,1],[1.18793,5.21917,23.6796,217.424],
    [1,1,1,1],[1.18793,5.21917,23.6796,217.424],
    [1,1,1,1],[1.18793,5.21917,23.6796,217.424]
],
"EN": [
    [1,1,1,1],[12.4061,90.7581,321.878,5215.5],
    [1,1,1,1],[12.4061,90.7581,321.878,5215.5],
    [1,1,1,1],[12.4061,90.7581,321.878,5215.5]
],
"SD": [
    [1,1,1,1],[48.4803,300.613,1014.27,10785.7],
    [1,1,1,1],[48.4803,300.613,1014.27,10785.7],
    [1,1,1,1],[48.4803,300.613,1014.27,10785.7]
],
"DB": [
    [1,1,1,1],[20.6244,207.121,469.131,2364.54],
    [1,1,1,1],[20.6244,207.121,469.131,2364.54],
    [1,1,1,1],[20.6244,207.121,469.131,2364.54]
],
"WF": [
    [1,1,1,1],[149.602,1489.21,2562.35,20147.1],
    [1,1,1,1],[149.602,1489.21,2562.35,20147.1],
    [1,1,1,1],[149.602,1489.21,2562.35,20147.1]
],
"ND": [
    [1,1,1,1],[605.324,2936.31,4917.54,14653.7],
    [1,1,1,1],[605.324,2936.31,4917.54,14653.7],
    [1,1,1,1],[605.324,2936.31,4917.54,14653.7]
],
"CP": [
    [1,1,1],[233.738,583.315,2101.5],
    [1,1,1],[233.738,583.315,2101.5],
    [1,1,1],[233.738,583.315,2101.5]
],
"LJ": [
    [1,1,1],[2344.87,3687.96,46440.4],
    [1,1,1],[2344.87,3687.96,46440.4],
    [1,1,1],[2344.87,3687.96,46440.4]
],
"WZ": [
    [],[],
    [],[],
    [],[]
],
}

UISim = {
"CG": [
    [1,1,1],[15.74,15.74,15.74]
],
"WV": [
    [1,1,1],[18.08,18.08,18.08]
],
"EN": [
    [1,1,1],[61.02,61.02,61.02]
],
"SD": [
    [1,1,1],[18.43,18.43,18.43]
],
"WF": [
    [0.999995,1,1],[7.72,7.72,7.72]
],
"ND": [
    [0.999843,1,1],[1904.82,1904.82,1904.82]
],
"DB": [
    [1,1,1],[14.54,14.54,14.54]
],
"CP": [
    [1,1,1],[123.43,123.43, 123.43]
],
"LJ": [
    [1,1,1],[406.68,406.68,406.68]
],
"WZ": [
    [1,1,1],[167.21,167.21,167.21]
],
}


algo_dic = {"R2LP": R2LP, "OptLP": OptLP, "UISim":UISim}


verts = [
   (-20, -20),
   (-20, 20),
   (20, 20),
   (20, -20),
   (-20, -20),
]

codes = [ Path.MOVETO, Path.LINETO, Path.LINETO, Path.LINETO, Path.CLOSEPOLY]
marker1 = Path(verts, codes)

verts = [(-20,- 20)]
codes = [Path.MOVETO]
for k in range(4):
    for i in range(5):
        if (k == 0): verts.append((-20, float((i + 1) * 8-20)))
        elif (k == 1): verts.append((float((i + 1) * 8-20), 20))
        elif(k==2): verts.append((20, float(20-(i+1)*8)))
        elif(k==3): verts.append((float(20-(i+1)*8), -20))

        if (i % 2 == 0): codes.append(Path.LINETO)
        else: codes.append(Path.MOVETO)
marker2 = Path(verts, codes)

verts = [(-20,-20)]
codes = [Path.MOVETO]
for k in range(4):
    for i in range(8):
        if (k == 0): verts.append((-20, float((i + 1) * 5)-20))
        elif (k == 1): verts.append((float((i + 1) * 5-20), 20))
        elif(k==2): verts.append((20, float(20-(i+1)*5)))
        elif(k==3): verts.append((float(20-(i+1)*5), -20))

        if (i % 2 == 0): codes.append(Path.LINETO)
        else: codes.append(Path.MOVETO)
marker3 = Path(verts, codes)

def plot_simrank_fig14(simrank_file):
    xlabel = 'Query Time'
    ylabel = 'Precision'
    title = "Fig14_"
    savedpath = './res/fig14/'
    if not os.path.exists(savedpath):
        os.makedirs(savedpath)


    for filename in simrank_file:
        fontsize = 60
        plt.figure(figsize=(24, 18))
        
        plt.xlabel(xlabel, fontsize=fontsize)  # , labelpad=15
        plt.ylabel(ylabel, fontsize=fontsize)  # , labelpad=15
        
        # plt.tick_params(labelsize=fontsize)  #
        plt.yticks(size=fontsize)
        plt.xticks(size=fontsize)
        plt.tick_params(axis='x', pad=20)


        plt.subplots_adjust(bottom=0.15)
        plt.subplots_adjust(left=0.20)
        
        ax = plt.gca()
        ax_width = 4
        ax.spines['bottom'].set_linewidth(ax_width)
        ax.spines['left'].set_linewidth(ax_width)
        ax.spines['right'].set_linewidth(ax_width)
        ax.spines['top'].set_linewidth(ax_width)

        # plt.yscale('log')
        plt.xscale('log')

        # 设置网格线粗细
        plt.grid(linewidth=3)
        
        plt.tick_params(axis="y", which="minor", direction="in", width=0, length=0, pad=3, labelsize=0)

        if (filename == 'WZ'):
            plt.xticks([300, 1000], [r'$3\times{10^{2}}$',r'$10^{3}$'])

        lineweight = 7.0
        markersize = 40
        marker_w = 7

        for algoname in algo_dic.keys():
            algo_data = algo_dic[algoname]
            t_s = time.time()

            linestyle = ['-', '--', ':']
            if(algoname=='R2LP'):
                for i in range(3):
                    xdata = algo_data[filename][i*2+1]
                    ydata = algo_data[filename][i*2]
                    if (len(xdata) == 0): continue
                    plt.plot(xdata, ydata, color="green", marker='o', linestyle=linestyle[i], linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
            elif (algoname == 'OptLP'):
                for i in range(3):
                    xdata = algo_data[filename][i*2+1]
                    ydata = algo_data[filename][i*2]
                    if (len(xdata) == 0): continue
                    plt.plot(xdata, ydata, color="red", marker='d', linestyle=linestyle[i], linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
            elif (algoname == 'UISim'):
                xdata = algo_data[filename][1]
                ydata = algo_data[filename][0]
                if (len(xdata) == 0): continue
                mymarker = [marker1, marker2, marker3]
                for i in range(3):
                    # plt.plot(xdata, ydata, color="m", marker='s', linestyle="", linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
                    plt.plot([xdata[i]], [ydata[i]], color="m", marker=mymarker[i], linestyle="", linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
            else: continue

            print(f"The result graph {filename} of the {algoname} has been drawn, cost {time.time() - t_s}s")


        t_s = time.time()
        print("Saving the data graph...")

        f = plt.gcf()
        respath = savedpath + "{}.png".format(title+filename)
        f.savefig(respath)
        f.clear()

        print(f"Data graph saved to {respath}, cost {time.time() - t_s}s")



if __name__ == "__main__":
    simrank_file = small_graph+middle_graph+large_graph
    
    plot_simrank_fig14(simrank_file)



