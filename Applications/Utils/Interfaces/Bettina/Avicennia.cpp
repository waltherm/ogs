/*
 * avicennia.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <Avicennia.h>

Avicennia::Avicennia(GeoLib::Point const &point, unsigned int id,
		Land &aLand, double age) :
		Tree(point, id, aLand,
				0.001,	//stemHeight
				0.04,	//crownHeight
				0.04,	//rootDepth
				0.3,	//crownRadius
				0.04,	//fineRootPermeability
				-7860,	//minimumLeafWaterPotential
				1.48,	//xylemConductivity
				0.1,	//halfMaxHeightGrowthWeight
				0.28,	//maintanceFactor
				age
				)
{
//	(GeoLib::Point const &point, unsigned int id, Land const &aLand,
//	double stemHeight, double crownHeight, double rootDepth,
//	double crownRadius, double fineRootPermeability,
//	double minimumLeafWaterPotential, double xylemConductivity,
//	double halfMaxHeightGrowthWeight, double maintanceFactor)

	// TODO Auto-generated constructor stub
}

Avicennia::~Avicennia() {
	// TODO Auto-generated destructor stub
}

