/*
 * flora.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: waltherm
 */

#include <random>
#include <boost/math/complex.hpp>

#include <Flora.h>
#include <time.h>

Flora::Flora(Land &aLand) :
		_treeCounter(0), _thisLand(aLand) {
	// TODO Auto-generated constructor stub
	initialPopulate();
}

Flora::~Flora() {
	// TODO Auto-generated destructor stub
}

void Flora::initialPopulate() {

//	//random distribution
//	for (std::size_t i(0); i < 10; i++) {
//		plantAviRandomRectangle(100, 100, 0);
//	}

// uniform distribution
	double const z = 0;
	for (std::size_t i(0); i < 20; i++) {
		double const x = i*4 +10;
		for (std::size_t j(0); j < 20; j++) {
			double const y = j*4 +10;
			plantAvi(x, y, z);
		}
	}

//// single tree distribution
//	plantAvi(31, 31, 0);
//	plantAvi(31.2, 31.2, 0);
//	plantAvi(30.8, 31.5, 0);

}

void Flora::plantAviRandomRectangle(double xMax, double yMax, double zMax,
		double xMin, double yMin, double zMin) {

	//Mersenne Twister: Good quality random number generator
	std::mt19937 rng;
	//Initialize with non-deterministic seeds
	rng.seed(std::random_device { }());

	std::uniform_real_distribution<double> uniformDoubleX(xMin, xMax);
	double const x(uniformDoubleX(rng));
	std::uniform_real_distribution<double> uniformDoubleY(yMin, yMax);
	double const y(uniformDoubleY(rng));
	std::uniform_real_distribution<double> uniformDoubleZ(zMin, zMax);
	double const z(uniformDoubleZ(rng));
	plantAvi(x, y, z);
}

void Flora::plantAviRandomCircle(double x, double y, double z, double radius) {
	//Mersenne Twister: Good quality random number generator
	std::mt19937 rng;
	//Initialize with non-deterministic seeds
	rng.seed(std::random_device { }());

	std::uniform_real_distribution<double> uniformDoubleAngle(0,
			2 * BettinaConstants::pi);
	double const angle(uniformDoubleAngle(rng));
	std::uniform_real_distribution<double> uniformDoubleDistance(0, radius);
	double const distance(
			uniformDoubleDistance(rng) * BettinaConstants::seedSpreadFactor
					* BettinaConstants::aviSizeFactor);
	double const xdiff(std::cos(angle) * distance);
	double const ydiff(std::sin(angle) * distance);
	plantAvi(x + xdiff, y + ydiff, z);
}

void Flora::plantAvi(double x, double y, double z) {
	GeoLib::Point const newTreePosition(x, y, z);
	_aliveTrees.push_back(
			new Avicennia(newTreePosition, _treeCounter++, _thisLand));	// guess, this is a bad way to get the tree counting correct
}


void Flora::recruitment() {

	const std::size_t noAliveTrees(_aliveTrees.size());
	for (std::size_t i(0); i < noAliveTrees; i++) {
		Tree *thisTree(_aliveTrees[i]);
		std::size_t seeds(thisTree->recruitment());
		for (std::size_t seed(0); seed < seeds; seed++) {
			// roll dice within crown radius (TODO: drift through wind?)
			// plant new trees
			plantAviRandomCircle(thisTree->getPosition().getCoords()[0],
					thisTree->getPosition().getCoords()[1],
					thisTree->getPosition().getCoords()[2],
					thisTree->getCrownRadius());
		}
	}

//		for (auto &aliveTree : _aliveTrees) {
//			std::size_t seeds(aliveTree->recruitment());
//
//			for (std::size_t seed(0); seed < seeds; seed++) {
//				// roll dice within crown radius (TODO: drift through wind?)
//				// plant new trees
//				plantAviRandomCircle(aliveTree->getPosition().getCoords()[0],
//						aliveTree->getPosition().getCoords()[1],
//						aliveTree->getPosition().getCoords()[2],
//						aliveTree->getCrownRadius());
//			}
//		}

}


void Flora::competition() {

	//TODO check calculation of competition - something seems wrong, at some point, many trees die suddenly
	//above ground competition
	_thisLand.resetAboveGroundCompetition();

	for (auto &aliveTree : _aliveTrees) {
		aliveTree->checkAboveGroundCompetition();
	}

	for (auto &aliveTree : _aliveTrees) {
		aliveTree->calcAboveGroundCompetition();
	}

	//below ground competition
	_thisLand.resetBelowGroundCompetition();

	for (auto &aliveTree : _aliveTrees) {
		aliveTree->checkBelowGroundCompetition();
	}

	_thisLand.invertBelowGroundCompetition();

	for (auto &aliveTree : _aliveTrees) {
		aliveTree->calcBelowGroundCompetition();
	}
}

void Flora::grow(double timeDiff) {
	for (auto &aliveTree : _aliveTrees) {
		aliveTree->grow();
		aliveTree->aging(timeDiff);
	}
}

void Flora::die() {

	for (auto & aliveTree : _aliveTrees) {
		if (aliveTree->getDeathFlag()) {
			_deadTrees.push_back(aliveTree); //copy aliveTrees with deathflag to deadTrees
			aliveTree = nullptr; //write nullpointer to aliveTree with deathflag
		}
	}
	_aliveTrees.erase( //remove element from vector from given begin (nullptr begin) to end (nullptr end)
			std::remove(_aliveTrees.begin(), _aliveTrees.end(), nullptr), //sort all nullptr elements to end of vector and return begin of nullptr elements
			_aliveTrees.end());

}

bool Flora::checkForActivePopulation() {
	if (_aliveTrees.empty()) {
		INFO("No alive trees left, exiting.");
		return false;
	} else
		return true;
}
