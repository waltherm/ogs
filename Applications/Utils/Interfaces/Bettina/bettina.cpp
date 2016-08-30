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
	std::string _fileName("/home/waltherm/03_scientific_work/mangroven/BETTINA/test/test.vtu");
	land _thisLand(_fileName);


	// create trees
	vector<tree*>_allTrees;

	GeoLib::Point const newPoint(30, 30, 0);	//TODO random sampling
	avicennia _singleAvi(newPoint, _thisLand.getSalinityAtPoint(newPoint), _allTrees.size(), _thisLand);
	_allTrees.push_back(&_singleAvi);

	// run through time steps
	for (unsigned int time(0); time < 10; time++)
	{
		for (unsigned int currentTree(0); currentTree < _allTrees.size(); currentTree++)
		{
			//_allTrees[currentTree]->competition();
			_allTrees[currentTree]->grow();
		}

		//TODO calculate Clark-Evans-Index
		_thisLand.updateSalinityAtPoint();	//TODO do for all trees
	}

	return 0;
}
