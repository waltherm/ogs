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

	thisOutput.writeFlora(thisFlora, 0);
	thisOutput.writeLand(thisLand, 0);

	// run through time steps
	double timeBegin(0), timeEnd(10000), timeDiff(1), currentTime(timeBegin);

	while (currentTime < timeEnd) {

		INFO("Time: %1.0f", currentTime);

		thisFlora.competition();
		thisFlora.grow(timeDiff);
		thisFlora.recruitment();
		thisFlora.die();

		thisOutput.writeFlora(thisFlora, currentTime+1);
		thisOutput.writeLand(thisLand, currentTime+1);

		if (!(thisFlora.checkForActivePopulation()))
			std::abort();

		//TODO calculate Clark-Evans-Index
		//TODO thisLand.updateSalinityAtPoint();

		currentTime+=timeDiff;

	}

	return 0;
}
