/*
 * rhizophora.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <Rhizophora.h>
#include "NearestNodeTable.h"

Rhizophora::Rhizophora(GeoLib::Point const &point, Land &aLand, double age) :
		Tree(point, aLand, getIniStemHeight(), getIniCrownHeight(),
				getIniRootDepth(), getIniCrownRadius(),
				getIniFineRootPermeability(), getIniMinimumLeafWaterPotential(),
				getIniXylemConductivity(), getIniHalfMaxHeightGrowthWeight(),
				getIniMaintenanceFactor(), age) {
// TODO Auto-generated constructor stub

	_crownRadiusNodeTable = new NearestNodeTable(this, NearestNodeTableClass::Crown);
	_rootRadiusNodeTable = new NearestNodeTable(this, NearestNodeTableClass::Root);
	_vicinityNodeTable = new NearestNodeTable(this, NearestNodeTableClass::Vicinity);

}

Rhizophora::~Rhizophora() {
	// TODO Auto-generated destructor stub
}

