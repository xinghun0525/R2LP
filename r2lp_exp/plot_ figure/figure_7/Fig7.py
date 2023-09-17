import matplotlib.pyplot as plt
import time
from matplotlib.path import Path
import matplotlib.patches as patches
import numpy as np

total = ["CG", "WV", "EN", "SD", "ND", "DB", "WF", "CP", "LJ", "WZ"]

wi = {
    "CG": [
        [0.00284317], [0.198675]
    ],
    "WV": [
        [0.000440174], [0.819112]
    ],
    "EN": [
        [0.00183704], [8.1644]
    ],
    "SD": [
        [0.000302547], [415.191]
    ],
    "DB": [
        [0.000828132], [334.995]
    ],
    "WF": [
        [0.00210915], [6.34813]
    ],
    "ND": [
        [0.000912021], [7693.27]
    ],
    "CP": [
        [0.00184477], [133.347]
    ],
    "LJ": [
        [0.00121773], [1451.49]
    ],
    "WZ": [
        [0.00217345], [590.601]
    ],
}
wio = {
    "CG": [
        [0.00355959], [0.332058]
    ],
    "WV": [
        [0.00052927], [1.92754]
    ],
    "EN": [
        [0.00196038], [41.5582]
    ],
    "SD": [
        [0.000348884], [1267.58]
    ],
    "DB": [
        [0.000844515], [513.627]
    ],
    "WF": [
        [0.00212547], [67.3416]
    ],
    "ND": [
        [0.00107706], [8572.31]
    ],
    "CP": [
        [0.00185315], [309.831]
    ],
    "LJ": [
        [0.00128889], [9606.96]
    ],
    "WZ": [
        [0.00220238], [3514.8]
    ]
}


algo_dic = {"wi": wi, "wio": wio}

total_marker = ['o', '^', 'd', 's', '<', 'p', '>', 'v', 'H', '*']

def plot_simrank_fig7(simrank_file, title_p):
    xlabel = 'Error'
    ylabel = 'Query Time'
    title = "Fig7_" + title_p
    savedpath = './'

    fontsize = 50
    plt.figure(figsize=(24, 18))
    
    plt.xlabel(xlabel, fontsize=fontsize)  # , labelpad=15
    plt.ylabel(ylabel, fontsize=fontsize)  # , labelpad=15


    # plt.tick_params(labelsize=fontsize)  #
    plt.yticks(size=fontsize)
    
    plt.xticks([0.0005, 0.0015, 0.0025, 0.0035], size=fontsize)
    # plt.xticks(size=fontsize)
    plt.tick_params(axis='x', pad=20)
    
    plt.subplots_adjust(bottom=0.15)
    plt.subplots_adjust(left=0.20)
    
    ax = plt.gca()
    ax_width = 4
    ax.spines['bottom'].set_linewidth(ax_width)
    ax.spines['left'].set_linewidth(ax_width)
    ax.spines['right'].set_linewidth(ax_width)
    ax.spines['top'].set_linewidth(ax_width)

    plt.yscale('log')

    plt.grid(linewidth=3)
    
    plt.tick_params(axis="y", which="minor", direction="in", width=0, length=0, pad=3, labelsize=0)
    lineweight = 7.0
    markersize = 60
    marker_w = 7

    marker = total_marker

    for i in range(len(simrank_file)):
        filename = simrank_file[i]
        for algoname in algo_dic.keys():
            algo_data = algo_dic[algoname][filename]
            t_s = time.time()

            xdata = algo_data[0]
            ydata = algo_data[1]
            if (len(xdata) == 0): continue
            
            xdata1, ydata1, xdata2, ydata2, xdata3, ydata3 = [], [], [], [], [], []
            if (len(algo_data) == 3):
                for ai in range(len(algo_data[2])):
                    if (algo_data[2][ai] == 0):
                        xdata1.append(xdata[ai])
                        ydata1.append(ydata[ai])
                    elif (algo_data[2][ai] == 1):
                        xdata2.append(xdata[ai])
                        ydata2.append(ydata[ai])
                    else:
                        xdata3.append(xdata[ai])
                        ydata3.append(ydata[ai])
            elif (len(algo_data) == 2):
                xdata1 = xdata
                ydata1 = ydata

            if (algoname == 'wi'):
                plt.plot(xdata, ydata, color="green", marker='', linestyle='-', linewidth=lineweight, label=algoname)
                if (len(xdata1) != 0):
                    plt.plot(xdata1, ydata1, color="green", marker=marker[i], linestyle='', linewidth=lineweight,
                             label=algoname, markersize=markersize, markerfacecolor='none', markeredgewidth=marker_w,
                             clip_on=False)
                if (len(xdata2) != 0):
                    plt.plot(xdata2, ydata2, color="green", marker=marker[i], linestyle='', linewidth=lineweight,
                             fillstyle='left',
                             label=algoname, markersize=markersize, markeredgewidth=marker_w, clip_on=False)
                if (len(xdata3) != 0):
                    plt.plot(xdata3, ydata3, color="green", marker=marker[i], linestyle='', linewidth=lineweight,
                             fillstyle='full',
                             label=algoname, markersize=markersize, markeredgewidth=marker_w, clip_on=False)
            elif (algoname == 'wio'):
                plt.plot(xdata, ydata, color="red", marker='', linestyle='-', linewidth=lineweight, label=algoname)
                if (len(xdata1) != 0):
                    plt.plot(xdata1, ydata1, color="red", marker=marker[i], linestyle='', linewidth=lineweight,
                             label=algoname,
                             markersize=markersize, markerfacecolor='none', markeredgewidth=marker_w, clip_on=False)
                if (len(xdata2) != 0):
                    plt.plot(xdata2, ydata2, color="red", marker=marker[i], linestyle='', linewidth=lineweight,
                             label=algoname, fillstyle='left',
                             markersize=markersize, markeredgewidth=marker_w, clip_on=False)
                if (len(xdata3) != 0):
                    plt.plot(xdata3, ydata3, color="red", marker=marker[i], linestyle='', linewidth=lineweight,
                             fillstyle='full',
                             label=algoname, markersize=markersize, markeredgewidth=marker_w, clip_on=False)
            else:
                continue
            print(f"The result graph {filename} of the {algoname} has been drawn, cost {time.time() - t_s}s")

    t_s = time.time()
    print("Saving the data graph...")
    f = plt.gcf()
    respath = savedpath + "{}.png".format(title)
    f.savefig(respath)
    f.clear()
    print(f"Data graph saved to {respath}, cost {time.time() - t_s}s")


if __name__ == "__main__":
    plot_simrank_fig7(total, 'Total')
