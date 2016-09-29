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

	void recruitment();
	void competition();
	void grow();
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
	void updateTreeIDs();

	std::vector<Tree*> _aliveTrees, _deadTrees;
	Land & _thisLand;
};

#endif /* FLORA_H_ */
