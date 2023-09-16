/*
 * InDegHub.cpp
 *
 *  Created on: 2019��3��16��
 *      Author: yichen_shen
 */

#include "InDegHub.h"
#include<iostream>


InDegHub::InDegHub(string nodeFile, string edgeFile):HubSelection(nodeFile,edgeFile){
}

void InDegHub::fillNodes() {

	for (auto &n : graph.getNodes())

		nodes.push_back(pair<int,double>(n->id, n->in.size()));
}

InDegHub::~InDegHub() {}

