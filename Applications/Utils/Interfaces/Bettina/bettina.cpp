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


#include "land.h"
#include "avicennia.h"

using namespace std;

int main() {

	ApplicationsLib::LogogSetup logog_setup;

	// reading
	std::string _fileName("/home/waltherm/03_scientific_work/mangroven/BETTINA/test/test.vtu");
	land _thisLand(_fileName);

	// create trees
	GeoLib::Point const newPoint(30, 30, 0);
	avicennia _singleAvi(newPoint, _thisLand.getSalinityAtPoint(newPoint));


	//std::cout << _thisLand.getSalinityAtPoint(30,30,0) << std::endl;



	return 0;
}
