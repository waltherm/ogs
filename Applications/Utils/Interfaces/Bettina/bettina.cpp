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
#include "Output.h"

using namespace std;

int main() {

	ApplicationsLib::LogogSetup logog_setup;

	std::string const salinityString = "Salinity", aboveGroundCompetitionString(
			"CompeteAbove"), belowGroundCompetitionString("CompeteBelow");

	// reading
	std::string const fileName(
			"/home/waltherm/03_scientific_work/mangroven/BETTINA/test/test.vtu");
	Output thisOutput(
			"/home/waltherm/03_scientific_work/mangroven/BETTINA/test/test");

	// create subsurface & flora
	Land thisLand(fileName, salinityString, aboveGroundCompetitionString,
			belowGroundCompetitionString);
	Flora thisFlora(thisLand);

	// run through time steps
	for (std::size_t time(0); time < 100000; time++) {

		thisFlora.recruitment();
		thisFlora.competition();
		thisFlora.grow();
		thisFlora.die();

		thisOutput.writeFlora(thisFlora, time);
		thisOutput.writeLand(thisLand.getSubsurface(), time);

		if (!(thisFlora.checkForActivePopulation()))
			std::abort();

		//TODO calculate Clark-Evans-Index
		//TODO thisLand.updateSalinityAtPoint();
	}

	return 0;
}
