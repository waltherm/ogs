/*
 * flora.cpp
 *
 *  Created on: Aug 30, 2016
 *      Author: waltherm
 */

#include <boost/math/complex.hpp>
//#include <boost/random/uniform_real.hpp>
//#include <boost/random/mersenne_twister.hpp>
//#include <boost/random/variate_generator.hpp>

#include <Flora.h>
#include <time.h>

Flora::Flora(Land &aLand) :
		_treeCounter(0), _random_number_generator(_rd()), _rnd(0, 1), _thisLand(
				aLand) {
	// TODO Auto-generated constructor stub
	initialPopulate();
}

Flora::~Flora() {
	// TODO Auto-generated destructor stub
	for (auto at : _aliveTrees)
		delete at;

	for (auto dt : _deadTrees)
		delete dt;
}

void Flora::initialPopulate() {

//	//random distribution
//	for (std::size_t i(0); i < 200; i++) {
//		plantAviRandomRectangle(100, 100, 0);
//	}

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
	plantAvi(2, 2, 0);
	plantAvi(4, 4, 0);

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

void Flora::plantTreeRandomCircle(double x, double y, double z, double radius,
		Tree const &aTree) {

	double const distance(
			radius * BettinaConstants::aviSizeFactor
					* _rnd(_random_number_generator));
	double const angle(
			2 * BettinaConstants::pi * _rnd(_random_number_generator));

	double const xdiff(std::cos(angle) * distance);
	double const ydiff(std::sin(angle) * distance);

	switch (aTree.getTreeType()) {
	case TreeType::Avicennia:
		plantAvi(x + xdiff, y + ydiff, z);
		break;
	default:
		ERR("Unknown tree type when planting seed.")
		;
		std::abort();
	}

}

void Flora::plantAviRandomCircle(double x, double y, double z, double radius) {
	double const distance(
			radius * BettinaConstants::aviSizeFactor
					* _rnd(_random_number_generator));
	double const angle(
			2 * BettinaConstants::pi * _rnd(_random_number_generator));

	double const xdiff(std::cos(angle) * distance);
	double const ydiff(std::sin(angle) * distance);
	plantAvi(x + xdiff, y + ydiff, z);
}

void Flora::plantAvi(double x, double y, double z) {
	GeoLib::Point const newTreePosition(x, y, z);
	_aliveTrees.push_back(
			new Avicennia(newTreePosition, _thisLand));	// TODO guess, this is a bad way to get the tree counting correct
}

void Flora::recruitment() {

	const std::size_t noAliveTrees(_aliveTrees.size());
	for (std::size_t i(0); i < noAliveTrees; i++) {
		std::size_t seeds(_aliveTrees[i]->recruitment());
		for (std::size_t seed(0); seed < seeds; seed++) {
			// roll dice within crown radius (TODO: drift through wind?)
			// plant new trees
			plantSeed(*_aliveTrees[i]);
		}
	}

}

void Flora::plantSeed(Tree const &aTree) {
	plantTreeRandomCircle(aTree.getPosition()[0], aTree.getPosition()[1],
			aTree.getPosition()[2],
			aTree.getCrownRadius() * BettinaConstants::seedSpreadFactor, aTree);
}

void Flora::competition() {		//memleak here

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
