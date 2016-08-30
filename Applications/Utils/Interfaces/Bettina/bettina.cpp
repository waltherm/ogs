//============================================================================
// Name        : BETTINA.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <iostream>

#include <logog/include/logog.hpp>
#include "Applications/ApplicationsLib/LogogSetup.h"

#include "Land.h"
#include "Flora.h"

using namespace std;

int main() {

	ApplicationsLib::LogogSetup logog_setup;

	// reading
	std::string fileName(
			"/home/waltherm/03_scientific_work/mangroven/BETTINA/test/test.vtu");

	// create subsurface & flora
	Land thisLand(fileName);
	Flora thisFlora(thisLand);


	// run through time steps
	for (std::size_t time(0); time < 100000; time++) {

		thisFlora.recruitment();
		thisFlora.competition();
		thisFlora.grow();
		thisFlora.die();
		if ( !(thisFlora.checkForActivePopulation()) )
			std::abort();

		//TODO calculate Clark-Evans-Index
		//TODO thisLand.updateSalinityAtPoint();
	}

	return 0;
}
