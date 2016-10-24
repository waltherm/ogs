/*
 * rhizophora.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef RHIZOPHORA_H_
#define RHIZOPHORA_H_

#include <Tree.h>

class Rhizophora: public Tree {
public:
	Rhizophora(GeoLib::Point const &point, Land &aLand);
	virtual ~Rhizophora();

	TreeType getTreeType() const override {
		return TreeType::Rhizophora;
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

	virtual double getSeedsPerUnitArea() const override {
		return 1;
	}
};

#endif /* RHIZOPHORA_H_ */
