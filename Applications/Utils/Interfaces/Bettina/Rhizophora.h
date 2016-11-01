/*
 * rhizophora.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef RHIZOPHORA_H_
#define RHIZOPHORA_H_

#include "Tree.h"


class Rhizophora: public Tree {
public:
	Rhizophora(GeoLib::Point const &point, Land &aLand, double age = 0);
	virtual ~Rhizophora();

	TreeType getTreeType() const override {
		return TreeType::Rhizophora;
	}

	double getTreeTypeDouble() const override {
			return 2;
	}

	double getMinSeedingAge() const override {
		return 30;
	}

	double getMinSeedingHeight() const override {
		return 12;
	}

	double getMinSeedingResources() const override {
		return 0.5e-3;
	}

	double getSeedsPerUnitArea() const override {
		return 2;
	}

	double getIniStemHeight() const override {
		return 0.002;
	}
	double getIniCrownHeight() const override {
		return 0.04;
	}
	double getIniRootDepth() const override {
		return 0.04;
	}
	double getIniCrownRadius() const override {
		return 0.35;
	}
	double getIniFineRootPermeability() const override {
		return 0.08;
	}
	double getIniMinimumLeafWaterPotential() const override {
		return -6860;
	}
	double getIniXylemConductivity() const override {
		return 1.78;
	}
	double getIniHalfMaxHeightGrowthWeight() const override {
		return 0.15;
	}
	double getIniMaintenanceFactor() const override {
		return 0.26;
	}

//private:
//	NearestNodeTable _crownRadiusNodeTable;
//	NearestNodeTable _rootRadiusNodeTable;
};

#endif /* RHIZOPHORA_H_ */
