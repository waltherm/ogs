/*
 * tree.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef TREE_H_
#define TREE_H_

#include "GeoLib/GEOObjects.h"

#include "Land.h"

class Tree {
public:
	Tree(GeoLib::Point const &point, unsigned int id, Land &aLand,
			double stemHeight, double crownHeight, double rootDepth,
			double crownRadius, double fineRootPermeability,
			double minimumLeafWaterPotential, double xylemConductivity,
			double halfMaxHeightGrowthWeight, double maintanceFactor);

	virtual ~Tree();

	void recruitment();
	void checkAboveGroundCompetition();
	void calcAboveGroundCompetition();
	void grow();
	double getSalinity() const;
	double getProperty(std::string propertyName) const;

	bool getDeathFlag() const {
		return _deathFlag;
	}

	const GeoLib::Point& getPosition() const {
		return _position;
	}

	void setStemHeightGrowth(double stemHeightGrowth) {
		_stemHeightGrowth = stemHeightGrowth;
	}

	void setStemHeightGrowthWeight(double stemHeightGrowthWeight) {
		_stemHeightGrowthWeight = stemHeightGrowthWeight;
	}

	std::size_t getId() const {
		return _id;
	}

	double getAboveGroundCompetitionCoefficient() const {
		return _aboveGroundCompetitionCoefficient;
	}

	double getAboveGroundResources() const {
		return _aboveGroundResources;
	}

	double getAvailableResources() const {
		return _availableResources;
	}

	double getBelowGroundCompetitionCoefficient() const {
		return _belowGroundCompetitionCoefficient;
	}

	double getBelowGroundResources() const {
		return _belowGroundResources;
	}

	double getBranchVolume() const {
		return _branchVolume;
	}

	double getCableRootVolume() const {
		return _cableRootVolume;
	}

	double getCrownHeight() const {
		return _crownHeight;
	}

	double getCrownRadius() const {
		return _crownRadius;
	}

	double getCrownRadiusGrowth() const {
		return _crownRadiusGrowth;
	}

	double getCrownRadiusGrowthWeight() const {
		return _crownRadiusGrowthWeight;
	}

	bool isDeathFlag() const {
		return _deathFlag;
	}

	double getDeathThreshold() const {
		return _deathThreshold;
	}

	double getFineRootGrowthWeight() const {
		return _fineRootGrowthWeight;
	}

	double getFineRootPermeability() const {
		return _fineRootPermeability;
	}

	double getFineRootVolume() const {
		return _fineRootVolume;
	}

	double getGrowth() const {
		return _growth;
	}

	double getGrowthLimitCoefficient() const {
		return _growthLimitCoefficient;
	}

	double getHalfMaxHeightGrowthWeigth() const {
		return _halfMaxHeightGrowthWeigth;
	}

	double getLateralFluxResistence() const {
		return _lateralFluxResistence;
	}

	double getLeafVolume() const {
		return _leafVolume;
	}

	double getMaintanceFactor() const {
		return _maintanceFactor;
	}

	double getMinimumLeafWaterPotential() const {
		return _minimumLeafWaterPotential;
	}

	std::size_t getNearestNodeId() const {
		return _nearestNodeID;
	}

	double getRadialFluxResistence() const {
		return _radialFluxResistence;
	}

	double getRootDepth() const {
		return _rootDepth;
	}

	double getRootRadius() const {
		return _rootRadius;
	}

	double getRootRadiusGrowth() const {
		return _rootRadiusGrowth;
	}

	double getSize() const {
		return _size;
	}

	double getSizeFactor() const {
		return _sizeFactor;
	}

	double getStemHeight() const {
		return _stemHeight;
	}

	double getStemHeightGrowth() const {
		return _stemHeightGrowth;
	}

	double getStemHeightGrowthWeight() const {
		return _stemHeightGrowthWeight;
	}

	double getStemRadius() const {
		return _stemRadius;
	}

	double getStemRadiusGrowth() const {
		return _stemRadiusGrowth;
	}

	double getStemRadiusGrowthWeight() const {
		return _stemRadiusGrowthWeight;
	}

	double getStemVolume() const {
		return _stemVolume;
	}

	double getStepFrac() const {
		return _stepFrac;
	}

	double getTreeVolume() const {
		return _treeVolume;
	}

	double getXylemConductivity() const {
		return _xylemConductivity;
	}

	double getCountabove() const {
		return countabove;
	}

	double getCountbelow() const {
		return countbelow;
	}

	double getMindist() const {
		return mindist;
	}

	double getWinCountabove() const {
		return win_countabove;
	}

	double getWinCountbelow() const {
		return win_countbelow;
	}

private:
	GeoLib::Point _position;
	double _stemRadius;	//r_stem
	double _crownRadius;	//r_crown
	double _rootRadius;	//r_root
	double _stemHeight;	//h_stem
	double _crownHeight;	//h_crown
	double _rootDepth;	//h_root
	double _leafVolume;	//v_leaf
	double _branchVolume;	//v_branch
	double _stemVolume;	//v_stem
	double _cableRootVolume;	//v_croot
	double _fineRootVolume;	//v_froot
	double _treeVolume;	//v_tree
	double _radialFluxResistence;//r1, Wasseraufnahmeẃiderstand, radialer Fluß
	double _lateralFluxResistence;//r2, Widerstand axialer Fluß (cable roots, Stamm, Äste)
	double _aboveGroundResources;	//res_a
	double _belowGroundResources;	//res_b
	double _availableResources;	//res_avail, minimum of _above and _belowGRes
	double _crownRadiusGrowthWeight;	//wa1;
	double _stemHeightGrowthWeight;	//wa2;
	double _fineRootGrowthWeight;	//wb1;
	double _stemRadiusGrowthWeight;	//wb2;
	double _growth;	//growth
	double _stemHeightGrowth;	//hg;
	double _crownRadiusGrowth;	//cg;
	double _rootRadiusGrowth;	//rg;
	double _stemRadiusGrowth;	//dg;
	double _stepFrac;	//stepfrac numerical helper variable
	double countabove;
	double win_countabove;
	double countbelow;
	double win_countbelow;
	double _aboveGroundCompetitionCoefficient;	//above_c
	double _belowGroundCompetitionCoefficient;	//below_c
	bool _deathFlag;	//deathflag
	double mindist;
	double _fineRootPermeability;	//L_p
	double _minimumLeafWaterPotential;	//psi_leaf
	double _xylemConductivity;	//k_f_sap, => permeability?
	double _halfMaxHeightGrowthWeigth;	//max_h, meaning of this parameter?
	double const _maintanceFactor;	//k_maint
	double const _growthLimitCoefficient;	//50 in "if growth > v_tree / 50 ["
	double const _deathThreshold;	//death.tresh, 0.004
	double _size;	//size
	double const _sizeFactor;	//size-factor

	std::size_t const _id;
	Land & _thisLand;
	std::size_t _nearestNodeID;

	void growTree();
	void calcGrowth();
	void growInVolume();
	void updateResistances();
	void gatherResources();
	void updateWeights();
	void updateGrowLengths();

	double stemHeightGrowth();
	double crownRadiusGrowth();
	double rootRadiusGrowth();
	double stemRadiusGrowth();

	//std::size_t findNearestNodeToTree();
	std::size_t findNearestNodeToTree() const;
	std::size_t findNearestNodeFromIDs(std::vector<std::size_t> nodeIDs) const;
	std::vector<std::size_t> const findNodesInRadius(double radius = -1.0) const;

	double getSalinityAtNearestNode() const;

	template<typename PROP_VAL_TYPE>
	PROP_VAL_TYPE getProperty(std::string propertyName) const {
		return getPropertyAtNearestNode<PROP_VAL_TYPE>(propertyName);
	}

	template<typename PROP_VAL_TYPE>
	PROP_VAL_TYPE getPropertyAtNearestNode(std::string propertyName) const {
		return _thisLand.getPropertyAtNodeID<PROP_VAL_TYPE>(_nearestNodeID,
				propertyName);
	}

};

#endif /* TREE_H_ */
