/*
 * tree.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <Globals.h>
#include <Tree.h>
#include "Land.h"

#include <cmath>

#include <iostream>

//#include "MeshGeoToolsLib/MeshNodeSearcher.h"
#include "MeshLib/Node.h"

Tree::Tree(GeoLib::Point const &point, unsigned int id, Land &aLand,
		double stemHeight, double crownHeight, double rootDepth,
		double crownRadius, double fineRootPermeability,
		double minimumLeafWaterPotential, double xylemConductivity,
		double halfMaxHeightGrowthWeight, double maintanceFactor) :
		_position(point), _crownRadius(crownRadius), _stemHeight(stemHeight), _crownHeight(
				crownHeight), _rootDepth(rootDepth), _leafVolume(-1), _branchVolume(
				-1), _stemVolume(-1), _cableRootVolume(-1), _fineRootVolume(-1), _treeVolume(
				-1), _radialFluxResistence(-1), _lateralFluxResistence(-1), _aboveGroundResources(
				-1), _belowGroundResources(-1), _availableResources(-1), _crownRadiusGrowthWeight(
				-1), _stemHeightGrowthWeight(-1), _fineRootGrowthWeight(-1), _stemRadiusGrowthWeight(
				-1), _growth(-1), _stemHeightGrowth(-1), _crownRadiusGrowth(-1), _rootRadiusGrowth(
				-1), _stemRadiusGrowth(-1), _stepFrac(0), _nodesWithinCrownRadius(
				1), _aboveGroundCompetitionWins(-1), _nodesWithinRootRadius(1), _belowGroundCompetitionWins(
				-1), _aboveGroundCompetitionCoefficient(1), _belowGroundCompetitionCoefficient(
				1), _deathFlag(false), mindist(-1), _fineRootPermeability(
				fineRootPermeability), _minimumLeafWaterPotential(
				minimumLeafWaterPotential), _xylemConductivity(
				xylemConductivity), _halfMaxHeightGrowthWeigth(
				halfMaxHeightGrowthWeight), _maintanceFactor(maintanceFactor), _growthLimitCoefficient(
				BettinaConstants::growthLimitCoefficient), _deathThreshold(
				BettinaConstants::deathTreshhold), _size(-1), _sizeFactor(
				BettinaConstants::aviSizeFactor), _id(id + 1), _thisLand(aLand), _nearestNodeID(
				findNearestNodeToTree()) {
	// TODO Auto-generated constructor stub
	// initializing

	double hir = 0
			- (_minimumLeafWaterPotential
					+ 9.81 * (_stemHeight + 2 * _crownRadius)
					+ 85 * getSalinity())
					/ (BettinaConstants::solarRadiation * BettinaConstants::pi
							* std::pow(_crownRadius, 2)) / 9.81 / 2;
	_rootRadius = 1.05
			/ std::pow(
					_fineRootPermeability * BettinaConstants::k_geom
							* BettinaConstants::pi * hir * _rootDepth, 0.5); //TODO: double-check functions
	_stemRadius = std::pow(
			(_stemHeight + BettinaConstants::oneOverSqrtTwo * _rootRadius
					+ 2 * _crownRadius)
					/ (_xylemConductivity * BettinaConstants::pi * hir), 0.5); //TODO: double-check functions
	_size = 2 * _crownRadius * _sizeFactor;

}

Tree::~Tree() {
	// TODO Auto-generated destructor stub
}

void Tree::recruitment() {
	// TODO: implement recruitment
}

void Tree::checkAboveGroundCompetition(std::vector<Tree*> &aliveTrees) {
	//findNodesInCrownRadius();
	_nodesWithinCrownRadius = findMinOneNodeInSearchRadius(
			_crownRadius * _sizeFactor);
	setAboveGroundCompetition(aliveTrees);
}

//void Tree::findNodesInCrownRadius() {
//
//	//find nodes in crown radius (if no nodes found, only use nearest node)
//	double searchRadius(_crownRadius * _sizeFactor);
//	std::vector<std::size_t> crownRadiusNodeIDs(
//			findNodesInRadius(searchRadius));
//
//	while (crownRadiusNodeIDs.size() < 1) {	//no nodes found within crown radius -> search for nearest node(s)
//		double const radiusIncrement(1.1);
//		searchRadius *= radiusIncrement;
//		crownRadiusNodeIDs = findNodesInRadius(searchRadius);
//		if (crownRadiusNodeIDs.size() < 1) {
//			continue;
//		} else {
//			if (crownRadiusNodeIDs.size() > 1) {//only search for nearest node
//				std::vector<std::size_t> temp_crownRadiusNodeIDs;
//				temp_crownRadiusNodeIDs.push_back(
//						findNearestNodeFromIDs(crownRadiusNodeIDs));
//				crownRadiusNodeIDs = temp_crownRadiusNodeIDs;
//			}
//		}
//	}
//
//	_nodesWithinCrownRadius = crownRadiusNodeIDs;
//
//}

std::vector<std::size_t> Tree::findMinOneNodeInSearchRadius(
		double searchRadius) {

	std::vector<std::size_t> searchRadiusNodeIDs(
			findNodesInRadius(searchRadius));

	while (searchRadiusNodeIDs.size() < 1) { //no nodes found within search radius -> search for nearest node(s)
		searchRadius *= BettinaConstants::searchRadiusIncrement;
		searchRadiusNodeIDs = findNodesInRadius(searchRadius);
		if (searchRadiusNodeIDs.size() < 1) {
			continue;
		} else {
			if (searchRadiusNodeIDs.size() > 1) { //only search for nearest node
				std::vector<std::size_t> temp_crownRadiusNodeIDs;
				temp_crownRadiusNodeIDs.push_back(
						findNearestNodeFromIDs(searchRadiusNodeIDs));
				searchRadiusNodeIDs = temp_crownRadiusNodeIDs;
			}
		}
	}

	return searchRadiusNodeIDs;

}

void Tree::setAboveGroundCompetition(std::vector<Tree*> &aliveTrees) {

	//in all nodes, set this tree ID as aboveGroundCompetitionID, if is highest
	for (std::size_t i(0); i < _nodesWithinCrownRadius.size(); i++) {
		double aboveGroundCompetition(
				_thisLand.getAboveGroundCompetitionAtNodeID(
						_nodesWithinCrownRadius[i]));
		if (aboveGroundCompetition < 0 || aboveGroundCompetition == _id) // no other tree or this tree already highest here
				{
			_thisLand.setAboveGroundCompetition(_id,
					_nodesWithinCrownRadius[i]);
		} else	// other tree here
		{
			double const otherTreesHeight(
					aliveTrees[aboveGroundCompetition]->getStemHeight()
							+ 2
									* aliveTrees[aboveGroundCompetition]->getCrownHeight());
			double const thisTreesHeight(_stemHeight + 2 * _crownHeight);
			if (otherTreesHeight <= thisTreesHeight)//this tree is higher or equal (and wins)
					{
				_thisLand.setAboveGroundCompetition(_id,
						_nodesWithinCrownRadius[i]);
			}
		}
	}

}

void Tree::calcAboveGroundCompetition() {

	//find nodes in crown radius (if no nodes found, only use nearest node)
	_aboveGroundCompetitionWins = 0;	// TODO could be local var
	for (auto id : _nodesWithinCrownRadius) {
		if (_thisLand.getAboveGroundCompetitionAtNodeID(id) == _id) {
			_aboveGroundCompetitionWins++;
		}
	}

	//calc relation between nodes where this tree is highest and all nodes within crownradius
	_aboveGroundCompetitionCoefficient = _aboveGroundCompetitionWins
			/ _nodesWithinCrownRadius.size();

	//calc above_c again
	// set above_c above_c + (1 - above_c) * (count patches in-radius ( 2 * 5 * size-factor ) with [compete-above < 0] ) / (2 * count patches in-radius ( 2 * 5 * size-factor ) )
	//count nodes, where no above competition in vicinity
	double searchRadius(10 * _sizeFactor);	// 10 is arbitrary magic number?
	std::vector<std::size_t> vicinityNodeIDs(findNodesInRadius(searchRadius));
	if (vicinityNodeIDs.size() > 0) {
		std::size_t vicinityNodes(0);
		for (auto id : vicinityNodeIDs) {
			if (_thisLand.getAboveGroundCompetitionAtNodeID(id) < 0) {
				vicinityNodes++;
			}
		}
		_aboveGroundCompetitionCoefficient += (1
				- _aboveGroundCompetitionCoefficient) * vicinityNodes
				/ (2 * vicinityNodeIDs.size());
	}

}

void Tree::checkBelowGroundCompetition() {
	// count nodes within rootRadius
	_nodesWithinRootRadius = findMinOneNodeInSearchRadius(
			_rootRadius * _sizeFactor);
	// increment belowGroundCompetition
	setBelowGroundCompetition();
}

void Tree::setBelowGroundCompetition() {
	for (auto id : _nodesWithinRootRadius) {
		_thisLand.incrementBelowGroundCompetition(id);
	}
}

void Tree::calcBelowGroundCompetition() {

	// calc belowGroundCompetitionCoefficient
	// set win_countbelow (max (list ((sum ([compete-below] of patches in-radius (r_root * size-factor)))) 0.5 ))
	_belowGroundCompetitionWins = 0;
	for (auto id : _nodesWithinRootRadius) {
		_belowGroundCompetitionWins +=
				_thisLand.getBelowGroundCompetitionAtNodeID(id);
	}

	_belowGroundCompetitionWins = std::max(_belowGroundCompetitionWins, 0.5);
	_belowGroundCompetitionCoefficient = std::min(
			_belowGroundCompetitionWins / _nodesWithinRootRadius.size(), 1.0);

}

void Tree::grow() {

	calcGrowth();
	_stepFrac = 0;

	if (_growth > _treeVolume / _growthLimitCoefficient) {
		while (_stepFrac < 1) {
			double const frac(
					std::min(_treeVolume / _growthLimitCoefficient / _growth,
							1 - _stepFrac));
			_growth *= frac;
			updateGrowLengths();
			growTree();
			_stepFrac += frac;
			if (_stepFrac > 1)
				std::cout << std::endl;
			calcGrowth();
		}
	} else {
		//	updateGrowLengths();
		growTree();
	}

}

void Tree::growTree() {
	_rootRadius += _rootRadiusGrowth;
	_crownRadius += _crownRadiusGrowth;
	_stemHeight += _stemHeightGrowth;
	_stemRadius += _stemRadiusGrowth;
}

void Tree::calcGrowth() {

	growInVolume();
	updateResistances();
	gatherResources();
	updateWeights();
	updateGrowLengths();

}

void Tree::growInVolume() {

	//	  set v_leaf pi * h_crown * r_crown ^ 2
	_leafVolume = BettinaConstants::pi * _crownHeight
			* std::pow(_crownRadius, 2);
	//	  set v_branch 2 * r_crown * pi * r_stem ^ 2
	_branchVolume = 2 * BettinaConstants::pi * _crownRadius
			* std::pow(_stemRadius, 2);
	//	  set v_stem pi * h_stem * r_stem ^ 2
	_stemVolume = BettinaConstants::pi * _stemHeight * std::pow(_stemRadius, 2);
	//	  set v_croot pi * 2 ^ (-0.5) * r_root * r_stem ^ 2
	_cableRootVolume = BettinaConstants::pi * BettinaConstants::oneOverSqrtTwo
			* _rootRadius * std::pow(_stemRadius, 2);
	//	  set v_froot pi * h_root * r_root ^ 2
	_fineRootVolume = BettinaConstants::pi * _rootDepth
			* std::pow(_rootRadius, 2);
	//	  set v_tree v_leaf + v_branch + v_stem + v_croot + v_froot
	_treeVolume = _leafVolume + _branchVolume + _stemVolume + _cableRootVolume
			+ _fineRootVolume;

}

void Tree::updateResistances() {

	//	  set r1 1 / (L_p * k_geom * pi * r_root ^ 2 * h_root)
	_radialFluxResistence = 1
			/ (_fineRootPermeability * BettinaConstants::k_geom
					* BettinaConstants::pi * std::pow(_rootRadius, 2)
					* _rootDepth);
	//	  set r2 (h_stem + 2 ^(-0.5) * r_root + 2 * r_crown) / (k_f_sap * pi * r_stem ^ 2)
	_lateralFluxResistence = (_stemHeight
			+ BettinaConstants::oneOverSqrtTwo * _rootRadius + 2 * _crownRadius)
			/ (_xylemConductivity * BettinaConstants::pi
					* std::pow(_stemRadius, 2));

}

void Tree::gatherResources() {

	//TODO compute competition

	//	  set res_a R_solar * pi * r_crown ^ 2  * above_c
	_aboveGroundResources = _aboveGroundCompetitionCoefficient
			* BettinaConstants::solarRadiation * BettinaConstants::pi
			* std::pow(_crownRadius, 2);
	//	  set res_b (0 - (psi_leaf + 9.81 * (h_stem + 2 * r_crown) + 85 * sal) / (r1 + r2) / 9.81) * below_c
	_belowGroundResources = _belowGroundCompetitionCoefficient
			* (0
					- (_minimumLeafWaterPotential
							+ BettinaConstants::gravityConstant
									* (_stemHeight + 2 * _crownRadius)
							+ 85 * getSalinity())	//TODO what is 85?
					/ (_radialFluxResistence + _lateralFluxResistence)
							/ BettinaConstants::gravityConstant);
	//	  set res_avail (min (list (k_rel * res_a)  res_b))
	_availableResources = std::min(
			BettinaConstants::k_rel * _aboveGroundResources,
			_belowGroundResources);
	//	  set growth k_grow * (res_avail - k_maint * v_tree)
	_growth = BettinaConstants::k_grow
			* (_availableResources - _maintanceFactor * _treeVolume);
//	  if growth < (v_tree / 100 * death.thresh / 100) [set deathflag 1]
	if (_growth < (_treeVolume * _deathThreshold)) {
		_deathFlag = true;
	}
}

void Tree::updateWeights() {

//	  let Q_res (k_rel * res_a - res_b) / (k_rel * res_a + res_b)
	double const qRes(
			(BettinaConstants::k_rel * _aboveGroundResources
					- _belowGroundResources)
					/ (BettinaConstants::k_rel * _aboveGroundResources
							+ _belowGroundResources));
//	  let Q_r   (r1 - r2) / (r1 + r2)
	double const qR(
			(_radialFluxResistence - _lateralFluxResistence)
					/ (_radialFluxResistence + _lateralFluxResistence));
//	  let Q_rad (r_crown - r_root) / (r_crown + r_root)
	double const qRad(
			(_crownRadius - _rootRadius) / (_crownRadius + _rootRadius));

//	  set wa2 max (list (max_h / (1 + exp((Qr0 - Q_rad) / sigmo_slope_hg))) 0)
	_stemHeightGrowthWeight =
			std::max(
					_halfMaxHeightGrowthWeigth
							/ (1
									+ std::exp(
											(BettinaConstants::Qr0 - qRad)
													/ BettinaConstants::sigmo_slope_hg)),
					0.0);
//	  set wa1 max (list ((1 - wa2) / (1 + exp(Q_res / sigmo_slope))) 0)
	_crownRadiusGrowthWeight = std::max(
			(1 - _stemHeightGrowthWeight)
					/ (1 + std::exp(qRes / BettinaConstants::sigmo_slope)),
			0.0);
//	  set wb2 max (list ((1 - wa1 - wa2) / (1 + exp(Q_r / sigmo_slope))) 0 )
	_stemRadiusGrowthWeight = std::max(
			(1 - _crownRadiusGrowthWeight - _stemHeightGrowthWeight)
					/ (1 + std::exp(qR / BettinaConstants::sigmo_slope)), 0.0);
//	  set wb1 1 - wa1 - wa2 - wb2
	_fineRootGrowthWeight = 1 - _crownRadiusGrowthWeight
			- _stemHeightGrowthWeight - _stemRadiusGrowthWeight;

}

void Tree::updateGrowLengths() {

	//TODO functions for individual equations
	_rootRadiusGrowth = rootRadiusGrowth();
	_crownRadiusGrowth = crownRadiusGrowth();
	_stemHeightGrowth = stemHeightGrowth();
	_stemRadiusGrowth = stemRadiusGrowth();

}

double Tree::rootRadiusGrowth() {
	//	  set rg wb1 * growth / (2 * pi * r_root * h_root + 2 ^ (-0.5) * pi * r_stem ^ 2)
	return _fineRootGrowthWeight * _growth
			/ (2 * BettinaConstants::pi * _rootRadius * _rootDepth
					+ BettinaConstants::oneOverSqrtTwo * BettinaConstants::pi
							* std::pow(_stemRadius, 2));
}

double Tree::crownRadiusGrowth() {
	//	  set cg wa1 * growth / (2 * pi * (r_crown * h_crown + r_stem ^ 2))
	return _crownRadiusGrowthWeight * _growth
			/ (2 * BettinaConstants::pi
					* (_crownRadius * _crownHeight + std::pow(_stemRadius, 2)));
}

double Tree::stemHeightGrowth() {
	//	  set hg wa2 * growth / (pi * r_stem ^ 2)
	return _stemHeightGrowthWeight * _growth
			/ (BettinaConstants::pi * std::pow(_stemRadius, 2));
}

double Tree::stemRadiusGrowth() {
	//	  set dg wb2 * growth / (2 * pi * r_stem * (h_stem + 2 ^ (-0.5) * r_root + 2 * r_crown))
	// 			 wb2 * growth / (2 * pi * r_stem * (h_stem + 2 ^ (-0.5) * r_root + 2 * r_crown))
	return _stemRadiusGrowthWeight * _growth
			/ (2 * BettinaConstants::pi * _stemRadius
					* (_stemHeight
							+ BettinaConstants::oneOverSqrtTwo * _rootRadius
							+ 2 * _crownRadius));
}

//std::size_t Tree::findNearestNodeToTree() {
//	MeshGeoToolsLib::SearchLength searchLength(
//			_thisLand.getSubsurface()->getMinEdgeLength());
//	MeshGeoToolsLib::MeshNodeSearcher _meshSearcher(*_thisLand.getSubsurface(),
//			searchLength);
//	auto idVector(_meshSearcher.getMeshNodeIDs(_position));
//	std::size_t nearestNodeID(-1);
//	if (idVector.size() == 0) {
//		ERR("No nodes found near tree no. %u.", _id);
//		std::abort();
//	} else {
//		if (idVector.size() == 1) {
//			nearestNodeID = idVector[0];
//		} else {
//			nearestNodeID = idVector[0]; //TODO: get nearest point from list of points
//		}
//	}
//	return nearestNodeID;
//}

std::size_t Tree::findNearestNodeToTree() const {

	std::vector<std::size_t> const idVector(findNodesInRadius());
	return findNearestNodeFromIDs(idVector);

}

std::size_t Tree::findNearestNodeFromIDs(
		std::vector<std::size_t> nodeIDs) const {

	std::size_t nearestNodeID(-1);
	if (nodeIDs.size() == 0) {
		ERR("No nodes found near tree no. %u.", _id);
		std::abort();
	} else {
		if (nodeIDs.size() == 1) {
			nearestNodeID = nodeIDs[0];
		} else {
			//TODO: return list of nearest nodes, if multiple points have same distance
			double distance(std::numeric_limits<double>::max());
			for (auto id : nodeIDs) {
				GeoLib::Point const idCoords(
						_thisLand.getSubsurface()->getNode(id)->getCoords()[0],
						_thisLand.getSubsurface()->getNode(id)->getCoords()[1],
						_thisLand.getSubsurface()->getNode(id)->getCoords()[2]);
				double thisDistance(
						std::sqrt(
								std::pow((_position[0] - idCoords[0]), 2)
										+ std::pow((_position[1] - idCoords[1]),
												2)
										+ std::pow((_position[2] - idCoords[2]),
												2)));
				if (thisDistance < distance) {
					distance = thisDistance;
					nearestNodeID = id;
				}
			}
		}
	}
	return nearestNodeID;
}

std::vector<std::size_t> const Tree::findNodesInRadius(
		double radius /*=-1.0*/) const {

	return _thisLand.findNodesInRadius(radius, _position);

}

double Tree::getSalinity() const {
	return getSalinityAtNearestNode();
}

double Tree::getSalinityAtNearestNode() const {
	return _thisLand.getSalinityAtNodeID(_nearestNodeID);
}

