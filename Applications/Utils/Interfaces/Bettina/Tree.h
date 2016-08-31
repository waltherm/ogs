/*
 * tree.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef TREE_H_
#define TREE_H_

#include "GeoLib/GEOObjects.h"

class Land;

class Tree {
public:
	Tree(GeoLib::Point const &point, unsigned int id, Land const &aLand,
			double stemHeight, double crownHeight, double rootDepth,
			double crownRadius, double fineRootPermeability,
			double minimumLeafWaterPotential, double xylemConductivity,
			double halfMaxHeightGrowthWeight,
			double maintanceFactor);

	virtual ~Tree();

	void recruitment();
	void competition();
	void grow();
	double getSalinity() const;

	bool getDeathFlag() const {
		return _deathFlag;
	}

	const GeoLib::Point& getPosition() const {
		return _position;
	}

	double getStemHeight() const {
		return _stemHeight;
	}

	double getStemHeightGrowth() const {
		return _stemHeightGrowth;
	}

	void setStemHeightGrowth(double stemHeightGrowth) {
		_stemHeightGrowth = stemHeightGrowth;
	}

	double getStemHeightGrowthWeight() const {
		return _stemHeightGrowthWeight;
	}

	void setStemHeightGrowthWeight(double stemHeightGrowthWeight) {
		_stemHeightGrowthWeight = stemHeightGrowthWeight;
	}

	std::size_t getId() const {
		return _id;
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
	double _radialFluxResistence;	//r1, Wasseraufnahmeẃiderstand, radialer Fluß
	double _lateralFluxResistence;	//r2, Widerstand axialer Fluß (cable roots, Stamm, Äste)
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
	Land const & _thisLand;
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

	std::size_t findNearestNodeToTree();
	double getSalinityAtNearestNode() const;

};

#endif /* TREE_H_ */
