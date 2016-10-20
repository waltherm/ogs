/*
 * flora.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: waltherm
 */

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

	std::srand(std::time(NULL));

//	// uniform distribution
//	double const z = 0;
//	for (std::size_t i(0); i < 20; i++) {
//		double const x = i*4 +10;
//		for (std::size_t j(0); j < 20; j++) {
//			double const y = j*4 +10;
//			GeoLib::Point const newPoint(x, y, z);
//			_aliveTrees.push_back(
//					new Avicennia(newPoint, _aliveTrees.size() - 1, _thisLand));
//		}
//	}

	// random distribution
	for (std::size_t i(0); i < 1000; i++) {
		double const x = 1 + std::rand() % 98;	//600
		double const y = 1 + std::rand() % 98;	//300
		double const z = 0;

		GeoLib::Point const newPoint(x, y, z);
		_aliveTrees.push_back(
				new Avicennia(newPoint, _aliveTrees.size() - 1, _thisLand));
	}

	// single tree distribution
//	GeoLib::Point const newPoint(31, 31, 0);
//	_aliveTrees.push_back(
//			new Avicennia(newPoint, _aliveTrees.size()-1, _thisLand));
//
//	GeoLib::Point const newPointb(31.2, 31.2, 0);
//	_aliveTrees.push_back(
//			new Avicennia(newPointb, _aliveTrees.size()-1, _thisLand));
//
//	GeoLib::Point const newPointc(30.8, 31.5, 0);
//	_aliveTrees.push_back(
//			new Avicennia(newPointc, _aliveTrees.size()-1, _thisLand));
}

void Flora::recruitment() {

	// check on adulthood
	//  possibility of sapling will rise with higher tree, more resource availability (not net availability)

	// roll dice within crown radius (TODO: wind?)

	// plant new trees


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

void Flora::grow() {
	for (auto &aliveTree : _aliveTrees) {
		aliveTree->grow();
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
