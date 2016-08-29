/*
 * avicennia.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <iostream>
#include "math.h"

#include "avicennia.h"
#include "globals.h"


avicennia::avicennia(GeoLib::Point point, double salinity)
{
	// TODO Auto-generated constructor stub
	setPosition(point);
	setStemHeight(0.0001);
	setCrownHeight(0.004);
	setRootDepth(0.004);
	setCrownRadius(0.3);
	setFineRootPermeability(0.04);
	setMinimumLeafWaterPotential(-7860);
	setXylemConductivity(1.48);
	setSolarRadiation(0.038);
	setHalfMaxHeightGrowthWeigth(0.1);
	setMaintanceFactor(0.28);

	globals global;

	double const pi = 3.14152653589;
	double hir = 0 - (_minimumLeafWaterPotential +
			9.81 * (_stemHeight + 2 * _crownRadius)
			+ 85 * salinity)
					/ (_solarRadiation * pi * std::pow(_crownRadius, 2))
					/ 9.81 / 2;
	setRootRadius( 1.05 / std::pow(_fineRootPermeability * global._k_geom * pi * hir * _rootDepth, 0.5) );	//TODO: double-check functions
	setStemRadius( std::pow( (_stemHeight + std::pow(2 , -0.5) * _rootRadius + 2 * _crownRadius) / (_xylemConductivity * pi * hir), 0.5) );	//TODO: double-check functions
	setSize(_crownRadius * _sizeFactor);
}

avicennia::~avicennia() {
	// TODO Auto-generated destructor stub
}

