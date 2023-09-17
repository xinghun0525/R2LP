import matplotlib.pyplot as plt
import time

small_graph = ["CG", "WV", "EN"]
middle_graph = ["SD", "ND", "DB", "WF"]
large_graph = ["CP", "LJ", "WZ"]

# The x-axis represents 'top_k', and the y-axis represents 'query time'. The values [0, 1, 2] correspond to normal termination, early termination, and accuracy below 0.9, respectively.
R2LP = {
"CG": [
    [1.44543,2.64637,1.42476,1.40033],['500', '1000', '5000', '10000']
],
"WV": [
    [10.8385,82.0683,24.6367,47.016],['500', '1000', '5000', '10000']
],
"EN": [
    [6.96513,6.90376,87.9975,284.901],['500', '1000', '5000', '10000']
],
"SD": [
    [160.59,276.12,2423.79,11563.6],['500', '1000', '5000', '10000']
],
"DB": [
    [404.493,103.174,333.681,631.138],['500', '1000', '5000', '10000']
],
"WF": [
    [66.4398,506.056,3487.75,6776.76],['500', '1000', '5000', '10000']
],
"ND": [
    [5359.89,2211.44,11748.7,23895.4],['500', '1000', '5000', '10000']
],
"CP": [
    [1126.15,1168.05,1189.9, 1062.77],['500', '1000', '5000', '10000'],[1,1,1,1]
],
"LJ": [
    [8952.64,8844.64,8823.6, 8926.14],['500', '1000', '5000', '10000'],[1,1,1,1]
],
"WZ": [
    [333.85,7277.46,7170.63, 3762.42],['500', '1000', '5000', '10000'],[0,1,1,0]
],
}
OptLP = {
"CG": [
    [2.21154,3.36472,2.15936,1.96466],['500', '1000', '5000', '10000']
],
"WV": [
    [76.5778,206.005,110.34,138.591],['500', '1000', '5000', '10000']
],
"EN": [
    [5.16668,5.45991,443.707,2185.53],['500', '1000', '5000', '10000']
],
"SD": [
    [1276.55,3095.04,7681.66,24798.3],['500', '1000', '5000', '10000'],[0,0,1,1]
],
"DB": [
    [464.997,227.721,1082.37,2052.37],['500', '1000', '5000', '10000']
],
"WF": [
    [1196.91,10927.2,10501.4,22157.4],['500', '1000', '5000', '10000'],[0,0,1,0]
],
"ND": [
    [1281.46,1277.93,4963.59,10335.9],['500', '1000', '5000', '10000']
],
"CP": [
    [2146.95,2121.8,2036.82,1882.4],['500', '1000', '5000', '10000'], [1,1,1,1]
],
"LJ": [
    [11106.8,10108.5,8585.63,8210.06],['500', '1000', '5000', '10000'], [1,1,1,2]
],
"WZ": [
    [],[]
]}


algo_dic = {"R2LP": R2LP, "OptLP": OptLP}
# Markers for different graphs.
small_marker = ['o', '^', 'd']
middle_marker1 = ['s', '<']
middle_marker2 = ['p', '>']
large_marker = ['v', 'H', '*']


def plot_simrank_fig9(simrank_file, title_p):
    xlabel = 'K'
    ylabel = 'Query Time'
    title = "Fig9_" + title_p
    savedpath = './fig9/'

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

    plt.yscale('log')

    plt.grid(linewidth=3)
    plt.tick_params(axis="y", which="minor", direction="in", width=0, length=0, pad=3, labelsize=0)
    lineweight = 7.0
    markersize = 60
    marker_w = 7

    marker = ['o', 'o', 'o']
    if(title_p == '(a) CG, WV, EN'):
        marker = small_marker
    elif(title_p == '(b) SD, ND'):
        marker = middle_marker1
    elif (title_p == '(c) DB, WF'):
        marker = middle_marker2
    elif (title_p == '(d) CP, LJ, WZ'):
        marker = large_marker

    for i in range(len(simrank_file)):
        filename = simrank_file[i]
        for algoname in algo_dic.keys():
            algo_data = algo_dic[algoname][filename]
            t_s = time.time()

            xdata = algo_data[1]
            ydata = algo_data[0]
            if (len(xdata) == 0): continue
            # Data represented by three different markers.
            xdata1, ydata1, xdata2, ydata2, xdata3, ydata3 = [],[],[],[],[],[]
            if(len(algo_data)==3):
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
            elif(len(algo_data)==2):
                xdata1 = xdata
                ydata1 = ydata

            if(algoname=='R2LP'):
                plt.plot(xdata, ydata, color="green", marker='', linestyle='-', linewidth=lineweight, label=algoname)
                if(len(xdata1) != 0):
                    plt.plot(xdata1, ydata1, color="green", marker=marker[i], linestyle='', linewidth=lineweight,
                             label=algoname, markersize=markersize, markerfacecolor='none', markeredgewidth=marker_w, clip_on=False)
                if(len(xdata2)!=0):
                    plt.plot(xdata2, ydata2, color="green", marker=marker[i], linestyle='', linewidth=lineweight, fillstyle='left',
                             label=algoname, markersize=markersize, markeredgewidth=marker_w, clip_on=False)
                if(len(xdata3)!=0):
                    plt.plot(xdata3, ydata3, color="green", marker=marker[i], linestyle='', linewidth=lineweight, fillstyle='full',
                             label=algoname, markersize=markersize, markeredgewidth=marker_w, clip_on=False)
            elif (algoname == 'OptLP'):
                plt.plot(xdata, ydata, color="red", marker='', linestyle='-', linewidth=lineweight, label=algoname)
                if(len(xdata1)!=0):
                    plt.plot(xdata1, ydata1, color="red", marker=marker[i], linestyle='', linewidth=lineweight, label=algoname,
                             markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
                if (len(xdata2) != 0):
                    plt.plot(xdata2, ydata2, color="red", marker=marker[i], linestyle='', linewidth=lineweight, label=algoname, fillstyle='left',
                             markersize=markersize, markeredgewidth=marker_w, clip_on=False)
                if (len(xdata3) != 0):
                    plt.plot(xdata3, ydata3, color="red", marker=marker[i], linestyle='', linewidth=lineweight, fillstyle='full',
                             label=algoname, markersize=markersize, markeredgewidth=marker_w, clip_on=False)
            else: continue
            print(f"{filename}-{algoname} plot, cost {time.time() - t_s}s")

    t_s = time.time()
    print("save figure...")
    f = plt.gcf()
    respath = savedpath + "{}.png".format(title)
    f.savefig(respath)
    f.clear()
    print(f"saved {respath}, cost {time.time() - t_s}s")



if __name__ == "__main__":
    plot_simrank_fig9(small_graph, '(a) CG, WV, EN')
    plot_simrank_fig9(middle_graph[:2], '(b) SD, ND')
    plot_simrank_fig9(middle_graph[2:], '(c) DB, WF')
    plot_simrank_fig9(large_graph, '(d) CP, LJ, WZ')



