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
    [0.820986,0.936952,0.964265,0.990782],[0.050184,0.200651,0.406046,1.66729],
    [0.66656,0.769516,0.830693,0.940711],[0.050184,0.200651,0.406046,1.66729],
    [0.195746,0.366368,0.46145,0.658918],[0.050184,0.200651,0.406046,1.66729]
],
"WV": [
    [1,1,1,1],[1.00507,1.13108,1.32654,4.2667],
[0.950551,0.952548,0.959404,0.984129],[1.00507,1.13108,1.32654,4.2667],
[0.866991,0.827992,0.836281,0.915016],[1.00507,1.13108,1.32654,4.2667]
],
"EN": [
    [0.992188,0.995791,0.99684,0.998016],[7.99674,12.8597,20.5388,132.234],
[0.93712,0.951478,0.957613,0.973446],[7.99674,12.8597,20.5388,132.234],
[0.515739,0.553603,0.592261,0.697361],[7.99674,12.8597,20.5388,132.234]
],
"SD": [
    [0.99931,0.999823,0.999887,0.999975],[22.5085,37.0945,63.6205,452.957],
[0.968205,0.990032,0.991362,0.993621],[22.5085,37.0945,63.6205,452.957],
[0.603376,0.690412,0.758834,0.882294],[22.5085,37.0945,63.6205,452.957]
],
"DB": [
    [0.874893,0.948446,0.969915,0.992832],[9.31714,41.0136,75.1262,328.059],
[0.762571,0.834914,0.877721,0.957125],[9.31714,41.0136,75.1262,328.059],
[0.299437,0.42236,0.506861,0.702409],[9.31714,41.0136,75.1262,328.059]
],
"WF": [
    [0.99009,0.993815,0.99697,0.998914],[9.62989,38.5576,78.421,399.126],
[0.793449,0.87427,0.904403,0.953266],[9.62989,38.5576,78.421,399.126],
[0.243683,0.397979,0.463169,0.617003],[9.62989,38.5576,78.421,399.126]
],
"ND": [
    [0.920049,0.977877,0.983834,0.993914],[429.713,2567.6,3767.71,8218.34],
[0.39082,0.774735,0.83493,0.92883],[429.713,2567.6,3767.71,8218.34],
[0.096882,0.319726,0.452084,0.738411],[429.713,2567.6,3767.71,8218.34]
],

"CP": [
    [0.988532,0.995413,1],[86.8034,122.792,481.532],
[0.760171,0.891579,0.983523],[86.8034,122.792,481.532],
[0.342459,0.503563,0.871234],[86.8034,122.792,481.532]
],
"LJ": [
    [0.967005,0.974619,0.994924],[670.386,1382.12,3070.25],
[0.886315,0.895854,0.934787],[670.386,1382.12,3070.25],
[0.582973,0.67903,0.749541],[670.386,1382.12,3070.25]
],
"WZ": [
    [0.999565,0.999565,0.999891],[538.561,849.529,3490.99],
[0.847226,0.862252,0.954478],[538.561,849.529,3490.99],
[0.55626,0.60082,0.754471],[538.561,849.529,3490.99]
],
}
OptLP = {
"CG": [
    [0.892796,0.966655,0.982816,0.99522],[0.173553,1.74211,2.79523,8.26265],
    [0.502078,0.816729,0.904281,0.974672],[0.173553,1.74211,2.79523,8.26265],
    [0.0880179,0.264956,0.44628,0.767326],[0.173553,1.74211,2.79523,8.26265]
],
"WV": [
    [1,1,1,1],[1.18793,5.21917,23.6796,217.424],
    [0.193641,0.954006,0.958,0.979702],[1.18793,5.21917,23.6796,217.424],
    [0.00403092,0.119777,0.404702,0.91731],[1.18793,5.21917,23.6796,217.424]
],
"EN": [
    [0.993092,0.996309,0.997279,0.999331],[12.4061,90.7581,321.878,5215.5],
    [0.696002,0.951279,0.960511,0.986353],[12.4061,90.7581,321.878,5215.5],
    [0.179117,0.362841,0.508359,0.76855],[12.4061,90.7581,321.878,5215.5]
],
"SD": [
    [0.999401,0.999895,0.999934,0.999984],[48.4803,300.613,1014.27,10785.7],
    [0.588426,0.989919,0.990348,0.994191],[48.4803,300.613,1014.27,10785.7],
    [0.11822,0.353006,0.547266,0.888792],[48.4803,300.613,1014.27,10785.7]
],
"DB": [
    [0.914108,0.973724,0.986272,0.997099],[20.6244,207.121,469.131,2364.54],
    [0.529199,0.857208,0.91802,0.979925],[20.6244,207.121,469.131,2364.54],
    [0.0966218,0.281588,0.455457,0.778161],[20.6244,207.121,469.131,2364.54]
],
"WF": [
    [0.99135,0.995857,0.998203,0.999635],[149.602,1489.21,2562.35,20147.1],
    [0.4968,0.892642,0.916914,0.980299],[149.602,1489.21,2562.35,20147.1],
    [0.049476,0.184842,0.357746,0.740302],[149.602,1489.21,2562.35,20147.1]
],
"ND": [
    [0.959966,0.990823,0.995524,0.999695],[605.324,2936.31,4917.54,14653.7],
    [0.441746,0.867393,0.9021,0.978206],[605.324,2936.31,4917.54,14653.7],
    [0.0936867,0.34293,0.535275,0.844495],[605.324,2936.31,4917.54,14653.7]
],
"CP": [
    [0.979358,1,1],[233.738,583.315,2101.5],
    [0.237998,0.522376,0.961147],[233.738,583.315,2101.5],
    [0.0527693,0.115822,0.42044],[233.738,583.315,2101.5]
],
"LJ": [
    [0.969543,0.984772,0.992386],[2344.87,3687.96,46440.4],
    [0.203037,0.452793,0.939459],[2344.87,3687.96,46440.4],
    [0.0308744,0.0688532,0.319845],[2344.87,3687.96,46440.4]
],
"WZ": [
    [],[],
    [],[],
    [],[]
],
}

UISim = {
"CG": [
    [0.864573,0.77167,0.599185],[15.74,15.74,15.74]
],
"WV": [
    [0.972603,0.963669,0.828193],[18.08,18.08,18.08]
],
"EN": [
    [0.902657,0.926692,0.511518],[61.02,61.02,61.02]
],
"SD": [
    [0.794319,0.666131,0.179493],[18.43,18.43,18.43]
],
"WF": [
    [0.807835,0.55597,0.0845198],[7.72,7.72,7.72]
],
"ND": [
    [0.879005,0.546714,0.167979],[1904.82,1904.82,1904.82]
],
"DB": [
    [0.611238,0.588707,0.135472],[14.54,14.54,14.54]
],
"CP": [
    [0.899083,0.61249	,0.20359],[123.43,123.43, 123.43]
],
"LJ": [
    [0.705584,0.563558,0.115683],[406.68,406.68,406.68]
],
"WZ": [
    [0.965776,0.705439,0.206484],[167.21,167.21,167.21]
],
}

algo_dic = {"R2LP": R2LP, "OptLP": OptLP, "UISim":UISim}


verts = [
   (0., 0.),
   (0., 45.),
   (45., 45.),
   (45., 0.),
   (0., 0.),
]

codes = [ Path.MOVETO, Path.LINETO, Path.LINETO, Path.LINETO, Path.CLOSEPOLY]
marker1 = Path(verts, codes)

verts = [(0.,0.)]
codes = [Path.MOVETO]
for k in range(4):
    for i in range(5):
        if (k == 0): verts.append((0., float((i + 1) * 9)))
        elif (k == 1): verts.append((float((i + 1) * 9), 45.))
        elif(k==2): verts.append((45., float(45-(i+1)*9)))
        elif(k==3): verts.append((float(45-(i+1)*9), 0))

        if (i % 2 == 0): codes.append(Path.LINETO)
        else: codes.append(Path.MOVETO)
marker2 = Path(verts, codes)

verts = [(0.,0.)]
codes = [Path.MOVETO]
for k in range(4):
    for i in range(9):
        if (k == 0): verts.append((0., float((i + 1) * 5)))
        elif (k == 1): verts.append((float((i + 1) * 5), 45.))
        elif(k==2): verts.append((45., float(45-(i+1)*5)))
        elif(k==3): verts.append((float(45-(i+1)*5), 0))

        if (i % 2 == 0): codes.append(Path.LINETO)
        else: codes.append(Path.MOVETO)
marker3 = Path(verts, codes)


def plot_simrank_fig15(simrank_file):
    xlabel = 'Query Time'
    ylabel = 'Recall'
    title = "Fig15_"
    savedpath = './res/fig15/'
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
                    plt.plot([xdata[i]], [ydata[i]], color="m", marker=mymarker[i], linestyle="", linewidth=lineweight, label=algoname, markersize=markersize+30, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
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
    
    plot_simrank_fig15(simrank_file)
