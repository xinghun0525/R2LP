# UISim2020

## How to Use

Run `UISim2020-main/UISim_C++/run.sh`.

### Switch to a new dataset

First, generate the Hubs file.

```shell
1. Modify the Config in `UISim_C++/src/Config.cpp` and compile with `make`.

2. ./SelectHubs [hubType]
```

Then, run the All-Pair algorithm

```shell
./Online_ap [hubType] [numHubs] [depth] [stopRea] [eta] [nodeFilepath] [edgeFilepath] [indexDirpath] [outputFilepath]
```

