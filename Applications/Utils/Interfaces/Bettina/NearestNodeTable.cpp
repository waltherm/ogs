/*
 * NearestNodeTable.cpp
 *
 *  Created on: Oct 25, 2016
 *      Author: waltherm
 */

#include "NearestNodeTable.h"
#include "Tree.h"
#include "Avicennia.h"

NearestNodeTable::NearestNodeTable(Tree *aTree, NearestNodeTableClass nntc) :
		_theTree(aTree), _nntc(nntc), _iniRadius(getIniRadius()), _minEdgeLength(
				getMinEdgeLength()) {
	initializeNearestNodeTable();
}

NearestNodeTable::~NearestNodeTable() {

}

void NearestNodeTable::initializeNearestNodeTable() {
	appendNearestNodeTable();
}

const std::vector<std::size_t> NearestNodeTable::getNearestNodeList() {
	std::size_t listID(
			std::floor((getCurrentRadius() - _iniRadius) / _minEdgeLength));
	/* TODO optimize node list:
	 * save search radius per list entry (also for extension of search radius)
	 * check whether current search radius is greater than last entry
	 * extend, if former is true
	 * for all found nodes in new search, check whether they are farther away from tree than last search radius and save only these
	 */
	while (listID >= _nearestNodeTable.size()) {
		appendNearestNodeTable();
	}
	return _nearestNodeTable[listID];
}

void NearestNodeTable::appendNearestNodeTable() {
	const std::vector<std::size_t> newNodes(
			_theTree->findMinOneNodeInSearchRadius(getCurrentRadius()));
	_nearestNodeTable.push_back(newNodes);
}

double NearestNodeTable::getIniRadius() const {
	//Avicennia *a=dynamic_cast<Avicennia*>(_theTree);	// will return nullptr
	double radius;
	switch (_nntc) {
	case NearestNodeTableClass::Crown:
		radius = _theTree->getIniCrownRadius() * _theTree->getSizeFactor();
		break;
	case NearestNodeTableClass::Root:
		radius = _theTree->getIniRootRadius() * _theTree->getSizeFactor();
		break;
	case NearestNodeTableClass::Vicinity:
		radius = _theTree->getSizeFactor();
		break;
	default:
		ERR("Wrong NearestNodeTableClass.");
		std::abort();
	}
	return radius;
}

double NearestNodeTable::getCurrentRadius() const {
	double radius;
	switch (_nntc) {
	case NearestNodeTableClass::Crown:
		radius = _theTree->getCrownRadius() * _theTree->getSizeFactor();
		break;
	case NearestNodeTableClass::Root:
		radius = _theTree->getRootRadius() * _theTree->getSizeFactor();
		break;
	case NearestNodeTableClass::Vicinity:
		radius = _theTree->getSizeFactor();
		break;
	default:
		ERR("Wrong NearestNodeTableClass.");
		std::abort();
	}
	return radius;
}

double NearestNodeTable::getMinEdgeLength() const {
	return _theTree->getThisLand().getSubsurface()->getMinEdgeLength();
}

//const std::vector<std::vector<std::size_t>>& NearestNodeTable::getNearestNodeTable() const {
//	return _nearestNodeTable;
//}

//void NearestNodeTable::addNodeList(std::vector<std::size_t> nearestNodeList) {
//	_nearestNodeTable.push_back(nearestNodeList);
//}
