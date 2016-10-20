/*
 * flora.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: waltherm
 */

#include <random>

#include <Flora.h>
#include <time.h>

Flora::Flora(Land &aLand) :
		_thisLand(aLand) {
	// TODO Auto-generated constructor stub
	initialPopulate();
}

Flora::~Flora() {
	// TODO Auto-generated destructor stub
}

void Flora::initialPopulate() {

	for (std::size_t i(0); i < 10; i++) {
		plantRandomAvi();
	}

//// uniform distribution
//	double const z = 0;
//	for (std::size_t i(0); i < 20; i++) {
//		double const x = i*4 +10;
//		for (std::size_t j(0); j < 20; j++) {
//			double const y = j*4 +10;
//			plantAvi(x, y, z);
//		}
//	}

//// single tree distribution
//	plantAvi(31, 31, 0);
//	plantAvi(31.2, 31.2, 0);
//	plantAvi(30.8, 31.5, 0);

}


void Flora::plantRandomAvi(double xMax, double yMax, double zMax, double xMin,
		double yMin, double zMin) {

	//Mersenne Twister: Good quality random number generator
	std::mt19937 rng;
	//Initialize with non-deterministic seeds
	rng.seed(std::random_device{}());

	std::uniform_real_distribution<double> unifx(xMin, xMax);
	double const x = unifx(rng);
	std::uniform_real_distribution<double> unify(yMin, yMax);
	double const y = unify(rng);
	std::uniform_real_distribution<double> unifz(zMin, zMax);
	double const z = unifz(rng);
	plantAvi(x, y, z);
}


void Flora::plantAvi(double x, double y, double z) {
	GeoLib::Point const newTreePosition(x, y, z);
	_aliveTrees.push_back(
			new Avicennia(newTreePosition, _aliveTrees.size() - 1, _thisLand));
}


void Flora::recruitment() {

	for (auto &aliveTree : _aliveTrees) {
		aliveTree->recruitment();
	}
}


void Flora::competition() {

	//TODO check calculation of competition - something seems wrong, at some point, many trees die suddenly
	//above ground competition
	_thisLand.resetAboveGroundCompetition();

	for (auto &aliveTree : _aliveTrees) {
		aliveTree->checkAboveGroundCompetition(_aliveTrees);
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

	updateTreeIDs();

}

void Flora::updateTreeIDs() {
	for (std::size_t i(0); i < _aliveTrees.size(); i++) {
		_aliveTrees[i]->setUpdatedID(i);
	}
}

bool Flora::checkForActivePopulation() {
	if (_aliveTrees.empty()) {
		INFO("No alive trees left, exiting.");
		return false;
	} else
		return true;
}
