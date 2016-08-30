/*
 * flora.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: waltherm
 */

#include <Flora.h>

Flora::Flora(Land const &aLand) :
		_thisLand(aLand) {
	// TODO Auto-generated constructor stub
	initialPopulate();
}

Flora::~Flora() {
	// TODO Auto-generated destructor stub
}

void Flora::initialPopulate() {

	GeoLib::Point const newPoint(30, 30, 0);	//TODO random sampling
	_aliveTrees.push_back(new Avicennia(newPoint, _aliveTrees.size(), _thisLand));
}

void Flora::recruitment() {
	for (auto &aliveTree : _aliveTrees) {
		aliveTree->recruitment();
	}
}

void Flora::competition() {
	for (auto &aliveTree : _aliveTrees) {
		aliveTree->competition();
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
			_deadTrees.push_back(aliveTree);	//copy aliveTrees with deathflag to deadTrees
			aliveTree = nullptr;	//write nullpointer to aliveTree with deathflag
		}
	}
	_aliveTrees.erase(	//remove element from vector from given begin (nullptr begin) to end (nullptr end)
			std::remove(_aliveTrees.begin(), _aliveTrees.end(), nullptr),	//sort all nullptr elements to end of vector and return begin of nullptr elements
			_aliveTrees.end());

}

bool Flora::checkForActivePopulation() {
	if (_aliveTrees.empty()) {
		INFO("No alive trees left, exiting.");
		return false;
	} else
		return true;
}
