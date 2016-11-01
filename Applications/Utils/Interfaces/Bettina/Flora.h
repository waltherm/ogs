/*
 * flora.h
 *
 *  Created on: Aug 30, 2016
 *      Author: waltherm
 */

#ifndef FLORA_H_
#define FLORA_H_

#include <vector>
#include <random>
//#include <boost/random/variate_generator.hpp>
//#include <boost/random/uniform_real.hpp>
//#include <boost/random/mersenne_twister.hpp>

//#include "Land.h"
#include "Tree.h"
#include "Avicennia.h"
#include "Rhizophora.h"

class Land;

class Flora {
public:
	Flora(Land &aLand);
	virtual ~Flora();

	void initialPopulate();
	bool checkForActivePopulation();

	void plantTreeRandomRectangle(TreeType treeType, double xMax,
			double yMax, double zMax, double xMin = 0, double yMin = 0,
			double zMin = 0);
	void plantTreeRandomCircle(TreeType treeType, double x, double y, double z,
			double radius);
	void plantSeed(Tree const &aTree);
	void plantAvi(double x, double y, double z);
	void plantRhi(double x, double y, double z);

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
	std::size_t _treeCounter;
	std::random_device _rd;
	std::mt19937 _random_number_generator;
	std::uniform_real_distribution<double> _rnd;

	std::vector<Tree*> _aliveTrees, _deadTrees;
	Land & _thisLand;
};

#endif /* FLORA_H_ */
