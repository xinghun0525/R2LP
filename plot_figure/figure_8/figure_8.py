import matplotlib.pyplot as plt
import time
from matplotlib.path import Path

small_graph = ["CG", "WV", "EN"]
middle_graph = ["SD", "DB", "WF", "ND"]
large_graph = ["CP", "LJ", "WZ"]

# "Each graph has three sets of data, corresponding to thres=0.1, 0.01, and 0.001, respectively."
R2LP = {
"CG": [
    [0.901694,0.967336,0.981808,0.99537],[0.050184,0.200651,0.406046,1.66729],
    [0.799902,0.869747,0.907518,0.969443],[0.050184,0.200651,0.406046,1.66729],
    [0.327365,0.535709,0.631403,0.794395],[0.050184,0.200651,0.406046,1.66729]
],
"WV": [
    [1,1,1,1],[1.00507,1.13108,1.32654,4.2667],
[0.974487,0.975616,0.979066,0.991596],[1.00507,1.13108,1.32654,4.2667],
[0.901855,0.905878,0.910785,0.955541],[1.00507,1.13108,1.32654,4.2667]
],
"EN": [
    [0.996074,0.997891,0.998417,0.999007,],[7.99674,12.8597,20.5388,132.234],
[0.967515,0.975132,0.978346,0.986544],[7.99674,12.8597,20.5388,132.234],
[0.680335,0.712269,0.7438,0.8217],[7.99674,12.8597,20.5388,132.234]
],
"SD": [
    [0.999653,0.999911,0.999943,0.999987],[22.5085,37.0945,63.6205,452.957],
[0.983615,0.994933,0.995631,0.9968],[22.5085,37.0945,63.6205,452.957],
[0.750496,0.80956,0.860452,0.937457],[22.5085,37.0945,63.6205,452.957]
],
"DB": [
    [0.933251,0.973501,0.984682,0.996403],[9.31714,41.0136,75.1262,328.059],
[0.865236,0.910012,0.934868,0.978091],[9.31714,41.0136,75.1262,328.059],
[0.460761,0.592571,0.672317,0.825185],[9.31714,41.0136,75.1262,328.059]
],
"WF": [
    [0.994996,0.996898,0.998483,0.999457],[9.62989,38.5576,78.421,399.126],
[0.883394,0.9328,0.94976,0.976071],[9.62989,38.5576,78.421,399.126],
[0.389597,0.566938,0.632499,0.76313],[9.62989,38.5576,78.421,399.126]
],
"ND": [
    [0.958284,0.988814,0.991851,0.996948],[429.713,2567.6,3767.71,8218.34],
[0.561704,0.873025,0.910035,0.963102],[429.713,2567.6,3767.71,8218.34],
[0.176639,0.484419,0.622607,0.849518],[429.713,2567.6,3767.71,8218.34]
],

"CP": [
    [0.994233,0.997701,1],[86.8034,122.792,481.532],
[0.844329,0.933944,0.989056],[86.8034,122.792,481.532],
[0.503665,0.644891,0.901905],[86.8034,122.792,481.532]
],
"LJ": [
    [0.983226,0.987147,0.997455],[670.386,1382.12,3070.25],
[0.939247,0.944872,0.965711],[670.386,1382.12,3070.25],
[0.691054,0.806391,0.851241],[670.386,1382.12,3070.25]
],
"WZ": [
    [0.999728,0.999783,0.999946],[538.561,849.529,3490.99],
[0.916123,0.925273,0.976586],[538.561,849.529,3490.99],
[0.675883,0.743526,0.854413],[538.561,849.529,3490.99]
],
}
OptLP = {
"CG": [
    [0.943362,0.983045,0.991333,0.997604],[0.173553,1.74211,2.79523,8.26265],
    [0.668511,0.89912,0.949735,0.987173],[0.173553,1.74211,2.79523,8.26265],
    [0.161795,0.418917,0.617142,0.868347],[0.173553,1.74211,2.79523,8.26265]
],
"WV": [
    [1,1,1,1],[1.18793,5.21917,23.6796,217.424],
    [0.324454,0.976461,0.97855,0.989747],[1.18793,5.21917,23.6796,217.424],
    [0.00802948,0.21393,0.576211,0.956872],[1.18793,5.21917,23.6796,217.424]
],
"EN": [
    [0.996534,0.998151,0.998637,0.999665],[12.4061,90.7581,321.878,5215.5],
    [0.820756,0.975031,0.979858,0.99313],[12.4061,90.7581,321.878,5215.5],
    [0.303816,0.532477,0.674056,0.86913],[12.4061,90.7581,321.878,5215.5]
],
"SD": [
    [0.9997,0.999947,0.999967,0.999992],[48.4803,300.613,1014.27,10785.7],
    [0.740892,0.994934,0.995151,0.997087],[48.4803,300.613,1014.27,10785.7],
    [0.211443,0.52181,0.707398,0.941122],[48.4803,300.613,1014.27,10785.7]
],
"DB": [
    [0.955127,0.986687,0.993089,0.998547],[20.6244,207.121,469.131,2364.54],
    [0.692125,0.923115,0.957258,0.989861],[20.6244,207.121,469.131,2364.54],
    [0.176217,0.439436,0.625861,0.875243],[20.6244,207.121,469.131,2364.54]
],
"WF": [
    [0.995656,0.997924,0.999101,0.999818],[149.602,1489.21,2562.35,20147.1],
    [0.663816,0.943276,0.956656,0.990051],[149.602,1489.21,2562.35,20147.1],
    [0.0942871,0.312011,0.52697,0.850774],[149.602,1489.21,2562.35,20147.1]
],
"ND": [
    [0.979415,0.994961,0.997053,0.996273],[605.324,2936.31,4917.54,14653.7],
    [0.612793,0.928982,0.948523,0.988908],[605.324,2936.31,4917.54,14653.7],
    [0.171323,0.510719,0.697302,0.915692],[605.324,2936.31,4917.54,14653.7]
],
"CP": [
    [0.989571,1,1],[233.738,583.315,2101.5],
    [0.384489,0.686264,0.980189],[233.738,583.315,2101.5],
    [0.100248,0.207599,0.591986],[233.738,583.315,2101.5]
],
"LJ": [
    [0.984536,0.992327,0.996178],[2344.87,3687.96,46440.4],
    [0.33754,0.623342,0.968785],[2344.87,3687.96,46440.4],
    [0.0598995,0.128836,0.48467],[2344.87,3687.96,46440.4]
],
"WZ": [
    [],[],
    [],[],
    [],[]
],
}
UISim = {
"CG": [
    [0.927368,0.871129,0.695959],[15.74,15.74,15.74]
],
"WV": [
    [0.986111,0.98147,0.906017],[18.08,18.08,18.08]
],
"EN": [
    [0.948796,0.961952, 0.676859],[61.02,61.02,61.02]
],
"SD": [
    [0.885371,0.799616,0.304355],[18.43,18.43,18.43]
],
"WF": [
    [0.893702,0.714631,0.155882],[7.72,7.72,7.72]
],
"ND": [
    [0.935538,0.706938,0.285647],[1904.82,1904.82,1904.82]
],
"DB": [
    [0.758718,0.741119,0.238635],[14.54,14.54,14.54]
],
"CP": [
    [0.94686,0.759682, 0.338305],[123.43,123.43, 123.43]
],
"LJ": [
    [0.827381,0.720867, 0.207376],[406.68,406.68,406.68]
],
"WZ": [
    [0.98259,0.827281, 0.34229],[167.21,167.21,167.21]
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

def plot_simrank_fig8(simrank_file):
    xlabel = 'Query Time'
    ylabel = 'F1-Score'
    title = "fig8_"
    savedpath = './fig8/'

    for filename in simrank_file:
        fontsize = 60
        plt.figure(figsize=(24, 18))

        plt.xlabel(xlabel, fontsize=fontsize) 
        plt.ylabel(ylabel, fontsize=fontsize) 

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
            print(f"{filename}-{algoname} plot, cost {time.time() - t_s}s")

        t_s = time.time()
        f = plt.gcf() 
        respath = savedpath + "{}.png".format(title+filename)
        f.savefig(respath)
        f.clear()
        print(f"saved {respath}, cost {time.time() - t_s}s")



if __name__ == "__main__":
    simrank_file = small_graph+middle_graph+large_graph
    plot_simrank_fig8(simrank_file)



