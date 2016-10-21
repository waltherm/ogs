/*
 * flora.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: waltherm
 */

#include <boost/math/complex.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

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
	for (std::size_t i(0); i < 2000; i++) {
		plantAviRandomRectangle(100, 100, 0);
	}

//// uniform distribution
//	double const z = 0;
//	for (std::size_t i(0); i < 20; i++) {
//		double const x = i * 4 + 10;
//		for (std::size_t j(0); j < 20; j++) {
//			double const y = j * 4 + 10;
//			plantAvi(x, y, z);
//		}
//	}

//// single tree distribution
//	plantAvi(31, 31, 0);
//	plantAvi(2, 2, 0);
//	plantAvi(4, 4, 0);

}

void Flora::plantAviRandomRectangle(double xMax, double yMax, double zMax,
		double xMin, double yMin, double zMin) {

	std::random_device rd;
	std::mt19937 random_number_generator(rd());

	std::uniform_real_distribution<double> uniformDoubleX(xMin, xMax);
	double const x(uniformDoubleX(random_number_generator));
	std::uniform_real_distribution<double> uniformDoubleY(yMin, yMax);
	double const y(uniformDoubleY(random_number_generator));
	std::uniform_real_distribution<double> uniformDoubleZ(zMin, zMax);
	double const z(uniformDoubleZ(random_number_generator));
	plantAvi(x, y, z);
}

void Flora::plantAviRandomCircle(double x, double y, double z, double radius,
		double angle) {

//	double const angle(
//			BettinaConstants::randomDoubleInRange(0, 2 * BettinaConstants::pi));
//	double const angle(
//			0 + (std::rand() / ( RAND_MAX / (2 * BettinaConstants::pi - 0))));

//	double const distance(
//			BettinaConstants::randomDoubleInRange(0,
//					radius * BettinaConstants::aviSizeFactor * BettinaConstants::seedSpreadFactor)); // TODO I dont like this sizefactor. why not making the crown 4 times bigger right away?
//	double const distance(
//			0
//					+ (std::rand()
//							/ ( RAND_MAX
//									/ (radius * BettinaConstants::aviSizeFactor
//											- 0))));

	double const xdiff(std::cos(angle) * radius);
	double const ydiff(std::sin(angle) * radius);
	plantAvi(x + xdiff, y + ydiff, z);
}

void Flora::plantAvi(double x, double y, double z) {
	GeoLib::Point const newTreePosition(x, y, z);
	_aliveTrees.push_back(
			new Avicennia(newTreePosition, _treeCounter++, _thisLand));	// TODO guess, this is a bad way to get the tree counting correct
}

void Flora::recruitment() {

//	std::srand(std::time(NULL));
	std::random_device rd;
	std::mt19937 random_number_generator(rd());
	std::uniform_real_distribution<double> rnd(0.0, 1.0);

	const std::size_t noAliveTrees(_aliveTrees.size());
	for (std::size_t i(0); i < noAliveTrees; i++) {
		std::size_t seeds(_aliveTrees[i]->recruitment());
		for (std::size_t seed(0); seed < seeds; seed++) {
			// roll dice within crown radius (TODO: drift through wind?)
			// plant new trees
			plantAviRandomCircle(_aliveTrees[i]->getPosition().getCoords()[0],
					_aliveTrees[i]->getPosition().getCoords()[1],
					_aliveTrees[i]->getPosition().getCoords()[2],
					_aliveTrees[i]->getCrownRadius()
							* BettinaConstants::seedSpreadFactor
							* BettinaConstants::aviSizeFactor
							* rnd(random_number_generator),
					2 * BettinaConstants::pi * rnd(random_number_generator));//* BettinaConstants::seedSpreadFactor	//FIXME why is this not working?
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
//			_deadTrees.push_back(aliveTree); //copy aliveTrees with deathflag to deadTrees
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
