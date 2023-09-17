import matplotlib.pyplot as plt
import time
from matplotlib.path import Path
import matplotlib.patches as patches
import numpy as np

small_graph = ["CG", "WV", "EN"]
middle_graph = ["SD", "ND", "DB", "WF"]
large_graph = ["CP", "LJ", "WZ"]

wi = {
    "CG": [
        [0.00449917, 0.00284317, 0.00217097, 0.00102582, ], [0.052363, 0.198675, 0.406707, 1.69186]
    ],
    "WV": [
        [0.000440174, 0.000423985, 0.000395695, 0.000190708], [0.819112, 1.08013, 1.48262, 4.81747]
    ],
    "EN": [
        [0.00183704, 0.00143154, 0.00122406, 0.000775305], [8.1644, 14.3147, 22.8516, 114.461]
    ],
    "SD": [
        [0.00105987, 0.000726146, 0.000551717, 0.000302547], [21.4206, 35.3529, 56.5899, 415.191]
    ],
    "DB": [
        [0.00347884, 0.00232365, 0.00178392, 0.000828132], [11.2928, 40.1119, 67.1632, 334.995]
    ],
    "WF": [
        [0.00210915, 0.00150723, 0.00128051, 0.000823873], [6.34813, 20.8705, 49.4709, 188.931]
    ],
    "ND": [
        [0.00543428, 0.00282578, 0.00207667, 0.000912021], [398.763, 1517.3, 3510.1, 7693.27]
    ],
    "CP": [
        [0.00297002, 0.00184477, 0.000382457], [83.9347, 133.347, 662.896]
    ],
    "LJ": [
        [0.00139074, 0.00121773, 0.000861637], [1019.93, 1451.49, 3308.74]
    ],
    "WZ": [
        [0.00217345, 0.00196557, 0.00103328], [590.601, 947.564, 2427.83]
    ],
}
wio = {
    "CG": [
        [0.00355959, 0.00152336, 0.000984263, 0.000316161], [0.332058, 1.03258, 1.91971, 6.03146]
    ],
    "WV": [
        [0.00052927, 0.000190848, 0.000135659, 5.47E-05], [1.92754, 4.31528, 5.57539, 11.2538]
    ],
    "EN": [
        [0.00196038, 0.000885948, 0.000609511, 0.000227975], [41.5582, 174.635, 265.617, 848.629]
    ],
    "SD": [
        [0.00151364, 0.000638075, 0.000348884, 0.000145408], [153.68, 754.595, 1267.58, 4081.69]
    ],
    "DB": [
        [0.00307763, 0.00129739, 0.000844515, 0.000276002], [73.488, 262.201, 513.627, 2016.53]
    ],
    "WF": [
        [0.00212547, 0.000922187, 0.000626776, 0.000240612], [67.3416, 196.226, 343.099, 1923.5]
    ],
    "ND": [
        [0.00487675, 0.0018597, 0.00107706, 0.000322551], [1761.79, 5100.14, 8572.31, 17125.7]
    ],
    "CP": [
        [0.00321772, 0.00185315, 0.00033454], [294.017, 309.831, 1352.01]
    ],
    "LJ": [
        [0.00186194, 0.00128889], [3924.91, 9606.96]
    ],
    "WZ": [
        [0.00220238, 0.00149994, 0.000553488], [3514.8, 6275.13, 25142]
    ]}


algo_dic = {"wi": wi, "wio": wio}

small_marker = ['o', '^', 'd']
middle_marker1 = ['s', '<']
middle_marker2 = ['p', '>']
large_marker = ['v', 'H', '*']
total_marker = ['o', '^', 'd', 's', '<', 'p', '>', 'v', 'H', '*']


def plot_simrank_fig13(simrank_file, title_p):
    xlabel = 'Error'
    ylabel = 'Query Time'
    title = "Fig13_" + title_p
    savedpath = './'

    fontsize = 50
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

    plt.yscale('log')

    plt.grid(linewidth=3)
    
    plt.tick_params(axis="y", which="minor", direction="in", width=0, length=0, pad=3, labelsize=0)
    lineweight = 7.0
    markersize = 60
    marker_w = 7

    marker = ['o', 'o', 'o']
    if (title_p == 'Small'):
        marker = small_marker
    elif (title_p == 'Middle1'):
        marker = middle_marker1
    elif (title_p == 'Middle2'):
        marker = middle_marker2
    elif (title_p == 'Large'):
        marker = large_marker

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
    # Draw merge image
    plot_simrank_fig13(small_graph, 'Small')
    plot_simrank_fig13(middle_graph[:2], 'Middle1')
    plot_simrank_fig13(middle_graph[2:], 'Middle2')
    plot_simrank_fig13(large_graph, 'Large')
