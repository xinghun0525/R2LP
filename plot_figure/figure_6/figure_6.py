import matplotlib.pyplot as plt
import time


small_graph = ["CG", "WV", "EN"]
middle_graph = ["SD", "DB", "WF", "ND"]
large_graph = ["CP", "LJ", "WZ"]

# The y-axis represents 'error,' and the x-axis represents 'time.'
R2LP = {
"CG": [
    [0.0044969,0.0028383,0.00215118,0.00103015,0.00021965],[0.050184,0.200651,0.406046,1.66729,7.57911]
],
"WV": [
    [0.00042254,0.00042492,0.00039959,0.00018826],[1.00507,1.13108,1.32654,4.2667]
],
"EN": [
    [0.0018368,0.00142955,0.00120178,0.00077546],[7.99674,12.8597,20.5388,132.234]
],
"ND": [
    [0.00543411,0.00281592,0.00207715,0.00091162],[429.713,2567.6,3767.71,8218.34]
],
"WF": [
    [0.00209793,0.00152204,0.00129017,0.00082335],[9.62989,38.5576,78.421,399.126]
],
"SD": [
    [0.00106451,0.0007235,0.00054983,0.00030041],[22.5085,37.0945,63.6205,452.957]
],
"DB": [
    [0.00347833,0.00232384,0.00178309,0.00082898],[9.31714,41.0136,75.1262,328.059]
],
"CP": [
    [0.00296035,0.0018453,0.00038975],[86.8034,122.792,481.532,]
],
"LJ": [
    [0.00145954,0.0012146,0.00085481],[670.386,1382.12,3070.25]],
"WZ": [
    [0.00217326,0.00196603,0.00099535],[538.561,849.529,3490.99]
],
}
FLP = {
"CG": [
    [0.00481025,0.00240927,0.00159824,0.000548102,6.65E-05],[0.17184,1.81399,2.48766,9.17476,87.6097]
],
"WV": [
    [0.00253475,0.00175577,0.000977481,0.000212598],[1.91016,10.688,46.2292,352.19]],
"EN": [
    [0.00369086,0.00175848,0.00120805,0.000518511],[23.5602,159.897,463.329,9014.7]
],
"ND": [
    [0.00469234,0.00210451,0.00137997,0.00045737],[1347.75,6526.97,11448.6,25603.7]
],
"WF": [
    [0.00305983,0.00183156,0.00129744,0.00048837],[209.763,1501.45,5111.32,28609.1]
],
"SD": [
    [0.00353971,0.00140633,0.000843602,0.000299132],[65.3297,637.219,1401.46,15051]
],
"DB": [
    [0.00445227,0.00216391,0.00143189,0.000491039],[40.6103,336.663,766.444,4545.21]
],
"CP": [
    [0.0057643,0.00405016,0.00130941],[562.997,1091.21,3435.28]
],
"LJ": [
    [0.0050133,0.00389548,0.00161983],[3307.59,5210.33,66737]
],
"WZ": [
    [0.00483602,0.00355822,0.00162584],[7264.48,11202.1,87829.3]
],
}
OptLP = {
"CG": [
    [0.00482414,0.00241559,0.00160413,0.0005566,6.88E-05],[0.173553,1.74211,2.79523,8.26265,41.2616]
],
"WV": [
    [0.00253443,0.00175585,0.00097766,0.00021488],[1.18793,5.21917,23.6796,217.424]
],
"EN": [
    [0.00369487,0.00176287,0.00121226,0.00052432],[12.4061,90.7581,321.878,5215.5]
],
"ND": [
    [0.00477416,0.00218344,0.00146776,0.000568475],[605.324,2936.31,4917.54,14653.7]
],
"WF": [
    [0.00305512,0.00182971,0.00129654,0.000512869],[149.602,1489.21,2562.35,20147.1]
],
"SD": [
    [0.00353892,0.00140498,0.00084206,0.00029955],[48.4803,300.613,1014.27,10785.7]
],
"DB": [
    [0.00447044,0.00217598,0.00144091,0.00050409],[20.6244,207.121,469.131,2364.54]
],
"CP": [
    [0.00574083,0.00399505,0.0013046],[233.738,583.315,2101.5]
],
"LJ": [
    [0.00500851,0.00389712,0.00162557],[2344.87,3687.96,46440.4]
],
"WZ": [
    [],[]
],
}
# The UISim data consists of the first two as hollow markers and the last two as solid markers.
UISim = {
"CG": [
    [0.00177817,0.00262935,0.00203754,0.00277738],[82.51,37.64,15.74,10.56]
],
"WV": [
    [0.000198671,0.000201095,0.000403848,0.000403918],[308.18,211.66,18.08,18.45]
],
"EN": [
    [0.0019663,0.0020296,0.0019358,0.00195912],[1831.12,1703.35,61.02,60.18]
],
"ND": [
    [0.00295814,0.00413165,0.00501329,0.00536688],[16716.57,5979.39,1904.82,1561.04]
],
"WF": [
    [0.00203665,0.00214537,0.00309307,0.00310512],[70.36,68.13,7.72,7.73]
],
"SD": [
    [0.000908105,0.000909843,0.00331611,0.00332059],[186.6,183.45,18.43,18.42]
],
"DB": [
    [0.00304146,0.00309638,0.00482541,0.00483179],[80.37,88.47,14.54,16.72]
],
"CP": [
    [0.00109598,0.00113122],[1326.04,1396.07]
],
"LJ": [
    [0.0012083,0.00121357],[5300.51,5231.82]
],
"WZ": [
    [0.00193344,0.00195],[2465.66,2152.18]
],
}
KSimJoin = {
"CG": [
    [0.00298745],[21.02]
],
"WV": [
    [0.0000847],[500.49]
],
"EN": [
    [],[]
],
"ND": [
    [],[]
],
"WF": [
    [],[]
],
"SD": [
    [],[]
],
"DB": [
    [],[]
],
"CP": [
    [],[]
],
"LJ": [
    [],[]
],
"WZ": [
    [],[]
],
}


algo_dic = {"R2LP": R2LP, "FLP":FLP, "OptLP": OptLP, "UISim":UISim, "KSimJoin":KSimJoin}



def plot_simrank_fig6(simrank_file):

    xlabel = 'Query Time'
    ylabel = 'Additive Error'
    title = "fig6_"
    savedpath = './fig6/'


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

        plt.yscale('log')
        plt.xscale('log')

        plt.grid(linewidth=3)
        plt.tick_params(axis="y", which="minor", direction="in", width=0, length=0, pad=3, labelsize=0)

        if(filename== 'CG'):
            plt.yticks([1e-4, 6e-4, 2e-3, 4e-3], [r'$1\times{10^{-4}}$', r'$6\times{10^{-4}}$', r'$2\times{10^{-3}}$', r'$4\times{10^{-3}}$'])
        elif(filename=='WV'):
            plt.yticks([2e-4, 5e-4, 1e-3, 2e-3], [r'$2\times{10^{-4}}$', r'$5\times{10^{-4}}$', r'$1\times{10^{-3}}$', r'$2\times{10^{-3}}$'])
        elif(filename=="EN"):
            plt.yticks([7e-4, 1e-3, 3e-3], [r'$7\times{10^{-4}}$', r'$1\times{10^{-3}}$', r'$ 3\times{10^{-3}}$'])
        elif(filename=="WF" or filename=="SD"):
            plt.yticks([5e-4, 1e-3, 2e-3], [r'$5\times{10^{-4}}$', r'$1\times{10^{-3}}$', r'$ 2\times{10^{-3}}$'])
        elif(filename=="CP" or filename == "DB" or filename=="LJ" or filename=="ND" or filename == "WZ"):
            plt.yticks([1e-3, 2e-3, 4e-3], [r'$1\times{10^{-3}}$', r'$2\times{10^{-3}}$', r'$ 3\times{10^{-3}}$'])

        lineweight = 7.0
        markersize = 50
        marker_w = 7

        for algoname in algo_dic.keys():
            algo_data = algo_dic[algoname]
            t_s = time.time()
            xdata = algo_data[filename][1]
            ydata = algo_data[filename][0]
            if(len(xdata)==0): continue

            if(algoname=='R2LP'):
                plt.plot(xdata, ydata, color="green", marker='o', linestyle="-", linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
            elif(algoname=='FLP'):
                plt.plot(xdata, ydata, color="blue", marker='^', linestyle="-", linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
            elif (algoname == 'OptLP'):
                plt.plot(xdata, ydata, color="red", marker='d', linestyle="-", linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
            elif (algoname == 'UISim'):
                # "Not filled represents the original stop reason, while filled represents five times the original stop reason."
                plt.plot(xdata[:2], ydata[:2], color="m", marker='s', linestyle="", linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
                if(len(xdata) ==4):
                    plt.plot(xdata[2:], ydata[2:], color="m", marker='s', linestyle="", linewidth=lineweight, label=algoname, markersize=markersize, markeredgewidth=marker_w, clip_on=False, mec = 'darkviolet')
            elif(algoname == 'KSimJoin'):
                plt.plot(xdata, ydata, color='darkgoldenrod', marker='p', linestyle="", linewidth=lineweight, label=algoname, markersize=markersize, markerfacecolor='none',markeredgewidth=marker_w, clip_on=False)
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

    plot_simrank_fig6(simrank_file)



