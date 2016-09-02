/*
 * avicennia.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <Avicennia.h>

Avicennia::Avicennia(GeoLib::Point const &point, unsigned int id,
		Land &aLand) :
		Tree(point, id, aLand,
				0.0001,	//stemHeight
				0.004,	//crownHeight
				0.004,	//rootDepth
				0.3,	//crownRadius
				0.04,	//fineRootPermeability
				-7860,	//minimumLeafWaterPotential
				1.48,	//xylemConductivity
				0.1,	//halfMaxHeightGrowthWeight
				0.28)	//maintanceFactor
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

