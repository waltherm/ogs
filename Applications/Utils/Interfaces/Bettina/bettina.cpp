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

#include "Land.h"
#include "Avicennia.h"
#include "Tree.h"

using namespace std;

int main() {

	ApplicationsLib::LogogSetup logog_setup;

	// reading
	std::string fileName(
			"/home/waltherm/03_scientific_work/mangroven/BETTINA/test/test.vtu");
	Land thisLand(fileName);

	// create Trees
	vector<Tree*> aliveTrees, deadTrees;
	GeoLib::Point const newPoint(30, 30, 0);	//TODO random sampling
	aliveTrees.push_back(new Avicennia(newPoint, aliveTrees.size(), thisLand));

	// run through time steps
	for (std::size_t time(0); time < 10; time++) {

		for (std::size_t i(0); i < aliveTrees.size(); i++) {
			Tree* thisTree = aliveTrees[i];
			thisTree->recruitment();
		}

		for (std::size_t i(0); i < aliveTrees.size(); i++) {
			Tree* thisTree = aliveTrees[i];
			thisTree->competition();
		}

		for (std::size_t i(0); i < aliveTrees.size(); i++) {
			Tree* thisTree = aliveTrees[i];
			thisTree->grow();
		}

		for (std::size_t i(0); i < aliveTrees.size(); i++) {
			Tree* thisTree = aliveTrees[i];
			if (thisTree->getDeathFlag()) {
				aliveTrees.erase(aliveTrees.begin() + i);
				deadTrees.push_back(thisTree);
				i--;
			}
			if (aliveTrees.size() < 1) {
				INFO("No alive Trees left, exiting.");
				std::abort();
			}
		}

		//TODO calculate Clark-Evans-Index
		//TODO thisLand.updateSalinityAtPoint();
	}

	return 0;
}
