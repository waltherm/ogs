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
	Rhizophora(GeoLib::Point const &point, unsigned int id, Land &aLand);
	virtual ~Rhizophora();

	double getMinSeedingAge() const override {
		return 20;
	}

	double getMinSeedingHeight() const override {
		return 10;
	}

	double getMinSeedingResources() const override {
		return 1e-3;
	}
};

#endif /* RHIZOPHORA_H_ */
