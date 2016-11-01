/*
 * avicennia.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef AVICENNIA_H_
#define AVICENNIA_H_

#include <Tree.h>


class Avicennia: public Tree {
public:
	Avicennia(GeoLib::Point const &point, Land &aLand, double age = 0);
	virtual ~Avicennia();

	TreeType getTreeType() const override {
		return TreeType::Avicennia;
	}

	double getTreeTypeDouble() const override {
			return 1;
	}

	double getMinSeedingAge() const override {
		return 20;
	}

	double getMinSeedingHeight() const override {
		return 10;
	}

	double getMinSeedingResources() const override {
		return 1e-3;
	}

	double getSeedsPerUnitArea() const override {
		return 0.3;
	}

	double getIniStemHeight() const override {
		return 0.001;
	}
	double getIniCrownHeight() const override {
		return 0.04;
	}
	double getIniRootDepth() const override {
		return 0.04;
	}
	double getIniCrownRadius() const override {
		return 0.3;
	}
	double getIniFineRootPermeability() const override {
		return 0.04;
	}
	double getIniMinimumLeafWaterPotential() const override {
		return -7860;
	}
	double getIniXylemConductivity() const override {
		return 1.48;
	}
	double getIniHalfMaxHeightGrowthWeight() const override {
		return 0.1;
	}
	double getIniMaintenanceFactor() const override {
		return 0.28;
	}

//private:
//	NearestNodeTable _crownRadiusNodeTable;
//	NearestNodeTable _rootRadiusNodeTable;
};

#endif /* AVICENNIA_H_ */
