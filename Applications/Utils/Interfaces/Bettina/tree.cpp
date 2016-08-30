/*
 * tree.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <cmath>

#include "MeshGeoToolsLib/MeshNodeSearcher.h"

#include "globals.h"
#include "tree.h"

tree::tree(GeoLib::Point const &point, unsigned int id, land const &aLand,
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
				-1), _stemRadiusGrowth(-1), _stepFrac(0), countabove(-1), win_countabove(
				-1), countbelow(-1), win_countbelow(-1), _aboveGroundCompetitionCoefficient(
				1), _belowGroundCompetitionCoefficient(1), _deathFlag(true), mindist(
				-1), _fineRootPermeability(fineRootPermeability), _minimumLeafWaterPotential(
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
			(_stemHeight + BettinaConstants::oneOverSQRTTwo * _rootRadius
					+ 2 * _crownRadius)
					/ (_xylemConductivity * BettinaConstants::pi * hir), 0.5); //TODO: double-check functions
	_size = _crownRadius * _sizeFactor;

}

tree::~tree() {
	// TODO Auto-generated destructor stub
}

void tree::grow() {

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
			calcGrowth();
		}
	} else {
		updateGrowLengths();
		growTree();
	}

}

void tree::growTree() {
	_rootRadius += _rootRadiusGrowth;
	_crownRadius += _crownRadiusGrowth;
	_stemHeight += _stemHeightGrowth;
	_stemRadius += _stemRadiusGrowth;
}

void tree::calcGrowth() {

	growInVolume();
	updateResistances();
	gatherResources();
	updateWeights();
	updateGrowLengths();

}

void tree::growInVolume() {

	//	  set v_leaf pi * h_crown * r_crown ^ 2
	_leafVolume = BettinaConstants::pi * _crownHeight
			* std::pow(_crownRadius, 2);
	//	  set v_branch 2 * r_crown * pi * r_stem ^ 2
	_branchVolume = 2 * BettinaConstants::pi * _crownRadius
			* std::pow(_stemRadius, 2);
	//	  set v_stem pi * h_stem * r_stem ^ 2
	_stemVolume = BettinaConstants::pi * _stemHeight * std::pow(_stemRadius, 2);
	//	  set v_croot pi * 2 ^ (-0.5) * r_root * r_stem ^ 2
	_cableRootVolume = BettinaConstants::pi * BettinaConstants::oneOverSQRTTwo
			* _rootRadius * std::pow(_stemRadius, 2);
	//	  set v_froot pi * h_root * r_root ^ 2
	_fineRootVolume = BettinaConstants::pi * _rootDepth
			* std::pow(_rootRadius, 2);
	//	  set v_tree v_leaf + v_branch + v_stem + v_croot + v_froot
	_treeVolume = _leafVolume + _branchVolume + _stemVolume + _cableRootVolume
			+ _fineRootVolume;

}

void tree::updateResistances() {

	//	  set r1 1 / (L_p * k_geom * pi * r_root ^ 2 * h_root)
	_radialFluxResistence = 1
			/ (_fineRootPermeability * BettinaConstants::k_geom
					* BettinaConstants::pi * std::pow(_rootRadius, 2)
					* _rootDepth);
	//	  set r2 (h_stem + 2 ^(-0.5) * r_root + 2 * r_crown) / (k_f_sap * pi * r_stem ^ 2)
	_lateralFluxResistence = (_stemHeight
			+ BettinaConstants::oneOverSQRTTwo * _rootRadius + 2 * _crownRadius)
			/ (_xylemConductivity * BettinaConstants::pi
					* std::pow(_stemRadius, 2));

}

void tree::gatherResources() {

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
							+ 85 * getSalinity())
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

void tree::updateWeights() {

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
	_stemHeightGrowthWeight = std::max(
			(_halfMaxHeightGrowthWeigth
					/ (1 + std::exp(BettinaConstants::Qr0 - qRad))
					/ BettinaConstants::sigmo_slope_hg), 0.0);
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

void tree::updateGrowLengths() {

	//TODO functions for individual equations
	_stemHeightGrowth = stemHeightGrowth();
	_crownRadiusGrowth = crownRadiusGrowth();
	_rootRadiusGrowth = rootRadiusGrowth();
	_stemRadiusGrowth = stemRadiusGrowth();

}

double tree::stemHeightGrowth() {
	//	  set hg wa2 * growth / (pi * r_stem ^ 2)
	return _stemHeightGrowthWeight * _growth
			/ (BettinaConstants::pi * std::pow(_stemRadius, 2));
}

double tree::crownRadiusGrowth() {
	//	  set cg wa1 * growth / (2 * pi * (r_crown * h_crown + r_stem ^ 2))
	return _crownRadiusGrowthWeight * _growth
			/ (2 * BettinaConstants::pi
					* (_crownRadius * _crownHeight + std::pow(_stemRadius, 2)));
}

double tree::rootRadiusGrowth() {
	//	  set rg wb1 * growth / (2 * pi * r_root * h_root + 2 ^ (-0.5) * pi * r_stem ^ 2)
	return _fineRootGrowthWeight * _growth
			/ (2 * BettinaConstants::pi * _rootRadius * _rootDepth
					+ BettinaConstants::oneOverSQRTTwo * BettinaConstants::pi
							* std::pow(_stemRadius, 2));
}

double tree::stemRadiusGrowth() {
	//	  set dg wb2 * growth / (2 * pi * r_stem * (h_stem + 2 ^ (-0.5) * r_root + 2 * r_crown))
	return _stemRadiusGrowthWeight * _growth
			/ (2 * BettinaConstants::pi * _stemRadius
					* (_stemHeight
							+ BettinaConstants::oneOverSQRTTwo * _rootRadius
							+ 2 * _crownRadius));
}

std::size_t tree::findNearestNodeToTree() {
	MeshGeoToolsLib::SearchLength searchLength(
			_thisLand.getMesh()->getMinEdgeLength());
	MeshGeoToolsLib::MeshNodeSearcher _meshSearcher(*_thisLand.getMesh(),
			searchLength);
	auto idVector(_meshSearcher.getMeshNodeIDs(_position));
	std::size_t nearestNodeID(-1);
	if (idVector.size() == 0) {
		ERR("No nodes found near tree no. %u.", _id);
		std::abort();
	} else {
		if (idVector.size() == 1) {
			nearestNodeID = idVector[0];
		} else {
			nearestNodeID = idVector[0]; //TODO: get nearest point from list of points
		}
	}
	return nearestNodeID;
}

double tree::getSalinity() const {
	return getSalinityAtNearestNode();
}

double tree::getSalinityAtNearestNode() const {
	return _thisLand.getSalinityAtNodeID(_nearestNodeID);
}
