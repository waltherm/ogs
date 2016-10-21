/*
 * flora.h
 *
 *  Created on: Aug 30, 2016
 *      Author: waltherm
 */

#ifndef FLORA_H_
#define FLORA_H_

#include <vector>

//#include "Land.h"
#include "Tree.h"
#include "Avicennia.h"

class Land;

class Flora {
public:
	Flora(Land &aLand);
	virtual ~Flora();

	void initialPopulate();
	bool checkForActivePopulation();

	void plantRandomAvi(double xMax = 100, double yMax = 100,
			double zMax = 0, double xMin = 0, double yMin = 0,
			double zMin = 0);
	void plantAvi(double x, double y, double z);

	void recruitment();
	void competition();
	void grow(double timeDiff);
	void die();

	std::size_t getNumberOfAliveTrees() const {
		return _aliveTrees.size();
	}

	const std::vector<Tree*>& getAliveTrees() const {
		return _aliveTrees;
	}

	const std::vector<Tree*>& getDeadTrees() const {
		return _deadTrees;
	}

private:
//	void updateTreeIDs();
	std::size_t _treeCounter;

	std::vector<Tree*> _aliveTrees, _deadTrees;
	Land & _thisLand;
};

#endif /* FLORA_H_ */
