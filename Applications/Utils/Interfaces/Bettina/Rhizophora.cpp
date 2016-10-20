/*
 * rhizophora.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <Rhizophora.h>

Rhizophora::Rhizophora(GeoLib::Point const &point, unsigned int id, Land &aLand) ://TODO check ini variables for rhizo
		Tree(point, id, aLand, 0.001,	//stemHeight
				0.04,	//crownHeight
				0.04,	//rootDepth
				0.3,	//crownRadius
				0.04,	//fineRootPermeability
				-7860,	//minimumLeafWaterPotential
				1.48,	//xylemConductivity
				0.1,	//halfMaxHeightGrowthWeight
				0.28	//maintanceFactor
				) {
// TODO Auto-generated constructor stub
}

Rhizophora::~Rhizophora() {
	// TODO Auto-generated destructor stub
}

