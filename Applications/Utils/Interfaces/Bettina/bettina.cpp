//============================================================================
// Name        : BETTINA.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description :
//============================================================================


#include <iostream>
#include <vector>
#include <logog/include/logog.hpp>
#include "Applications/ApplicationsLib/LogogSetup.h"


#include "land.h"
#include "avicennia.h"
#include "tree.h"

using namespace std;

int main() {

	ApplicationsLib::LogogSetup logog_setup;

	// reading
	std::string fileName("/home/waltherm/03_scientific_work/mangroven/BETTINA/test/test.vtu");
	land thisLand(fileName);


	// create trees
	vector<tree*>allTrees;
	GeoLib::Point const newPoint(30, 30, 0);	//TODO random sampling
	allTrees.push_back(new avicennia(newPoint, allTrees.size(), thisLand));

	// run through time steps
	for (std::size_t time(0); time < 10; time++)
	{
		for (std::size_t currentTree(0); currentTree < allTrees.size(); currentTree++)
		{
			//allTrees[currentTree]->competition();
			allTrees[currentTree]->grow();
		}

		//TODO calculate Clark-Evans-Index
		//thisLand.updateSalinityAtPoint();	//TODO do for all trees
	}

	return 0;
}
