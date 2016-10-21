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
	Avicennia(GeoLib::Point const &point, unsigned int id, Land &aLand,
			double age = 0);
	virtual ~Avicennia();

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
		return 0.3;
	}

};

#endif /* AVICENNIA_H_ */
