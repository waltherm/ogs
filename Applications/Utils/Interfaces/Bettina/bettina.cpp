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

	std::string const salinityString = "Salinity2", aboveGroundCompetitionString(
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

	thisOutput.writeFlora(thisFlora, 0);
	thisOutput.writeLand(thisLand.getSubsurface(), 0);

	// run through time steps
	for (std::size_t time(0); time < 100000; time++) {

		thisFlora.recruitment();
		thisFlora.competition();
		thisFlora.grow();
		thisFlora.die();

		thisOutput.writeFlora(thisFlora, time+1);
		thisOutput.writeLand(thisLand.getSubsurface(), time+1);

		if (!(thisFlora.checkForActivePopulation()))
			std::abort();

		//TODO calculate Clark-Evans-Index
		//TODO thisLand.updateSalinityAtPoint();
		INFO("Time: %i", time);
	}

	return 0;
}
