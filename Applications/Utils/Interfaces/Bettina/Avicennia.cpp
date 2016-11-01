/*
 * avicennia.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <Avicennia.h>
#include "NearestNodeTable.h"

Avicennia::Avicennia(GeoLib::Point const &point, Land &aLand, double age) :
		Tree(point, aLand, getIniStemHeight(), getIniCrownHeight(),
				getIniRootDepth(), getIniCrownRadius(),
				getIniFineRootPermeability(), getIniMinimumLeafWaterPotential(),
				getIniXylemConductivity(), getIniHalfMaxHeightGrowthWeight(),
				getIniMaintenanceFactor(), age) {

	_crownRadiusNodeTable = new NearestNodeTable(this, NearestNodeTableClass::Crown);
	_rootRadiusNodeTable = new NearestNodeTable(this, NearestNodeTableClass::Root);
	_vicinityNodeTable = new NearestNodeTable(this, NearestNodeTableClass::Vicinity);

}

Avicennia::~Avicennia() {

}


