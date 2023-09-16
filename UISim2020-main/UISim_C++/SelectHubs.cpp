/*
 * SelectHubs.cpp
 *
 *  Created on: 2019��3��17��
 *      Author: yichen_shen
 */

#include "include/Config.h"
#include "include/HubSelection.h"
#include "include/InDegHub.h"
#include "include/RandomHub.h"
#include <iostream>
#include "include/Trim.h"
#include <cstring>

using namespace std;


int main(int argc, char *argv[])
{

	Config::hubType = argv[1];
	if (strcmp(argv[1], "random") == 0)
	{
		RandomHub h(Config::nodeFile, Config::edgeFile);
		h.save();
	}
	else if (strcmp(argv[1], "indeg") == 0)
	{
		InDegHub h(Config::nodeFile, Config::edgeFile);
		h.save();
	}
	else
	{
		cout << "Unknown hub selection algorithm!" << endl;
		return 0;
	}
	return 0;
}
