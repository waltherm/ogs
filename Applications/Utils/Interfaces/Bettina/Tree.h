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
#include "Globals.h"
#include "NearestNodeTable.h"

enum class TreeType {
	Avicennia, Rhizophora
};

class Tree {
public:
	Tree(GeoLib::Point const &point, Land &aLand, double stemHeight,
			double crownHeight, double rootDepth, double crownRadius,
			double fineRootPermeability, double minimumLeafWaterPotential,
			double xylemConductivity, double halfMaxHeightGrowthWeight,
			double maintanceFactor, double age = 0);

	virtual ~Tree();

	std::size_t recruitment();

	void checkAboveGroundCompetition();
	std::vector<std::size_t> findMinOneNodeInSearchRadius(double searchRadius);
	void setAboveGroundCompetition();
	void calcAboveGroundCompetition();

	void checkBelowGroundCompetition();
	void setBelowGroundCompetition();
	void calcBelowGroundCompetition();

	void grow();
	void aging(double timeDiff);
	void increaseAge(double timeDiff);

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

	std::size_t getID() const {
		return _ID;
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
		return _halfMaxHeightGrowthWeight;
	}

	double getLateralFluxResistence() const {
		return _lateralFluxResistence;
	}

	double getLeafVolume() const {
		return _leafVolume;
	}

	double getMaintanceFactor() const {
		return _maintenanceFactor;
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

	std::vector<std::size_t> getNodesWithinCrownRadius() const {
		return _nodesWithinCrownRadius;
	}

	std::vector<std::size_t> getNodesWithinRootRadius() const {
		return _nodesWithinRootRadius;
	}

	double getMindist() const {
		return mindist;
	}

	double getWinCountabove() const {
		return _aboveGroundCompetitionWins;
	}

	double getWinCountbelow() const {
		return _belowGroundCompetitionWins;
	}

	double getAge() const {
		return _age;
	}

	static std::size_t getNumberOfTrees() {
		return _numberOfTrees;
	}

	double getIniRootRadius() const {
		return _iniRootRadius;
	}

	const Land getThisLand() const {
		return _thisLand;
	}

	virtual TreeType getTreeType() const = 0;
	virtual double getMinSeedingAge() const = 0; // this function needs to be implemented in the derived classes
	virtual double getMinSeedingHeight() const = 0;
	virtual double getMinSeedingResources() const = 0;
	virtual double getSeedsPerUnitArea() const = 0;
	virtual double getIniStemHeight() const = 0;
	virtual double getIniCrownHeight() const = 0;
	virtual double getIniRootDepth() const = 0;
	virtual double getIniCrownRadius() const = 0;
	virtual double getIniFineRootPermeability() const = 0;
	virtual double getIniMinimumLeafWaterPotential() const = 0;
	virtual double getIniXylemConductivity() const = 0;
	virtual double getIniHalfMaxHeightGrowthWeight() const = 0;
	virtual double getIniMaintenanceFactor() const = 0;


private:
	static std::size_t _numberOfTrees;

	GeoLib::Point _position;
	double _age;
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
	std::vector<std::size_t> _nodesWithinCrownRadius;//countabove number of nodes within crownradius * sizefactor
	double _aboveGroundCompetitionWins;
	std::vector<std::size_t> _nodesWithinRootRadius;//countbelow number of nodes within rootradius * sizefactor
	double _belowGroundCompetitionWins;
	double _aboveGroundCompetitionCoefficient;	//above_c
	double _belowGroundCompetitionCoefficient;	//below_c
	bool _deathFlag;	//deathflag
	double mindist;
	double _fineRootPermeability;	//L_p
	double _minimumLeafWaterPotential;	//psi_leaf
	double _xylemConductivity;	//k_f_sap, => permeability?
	double _halfMaxHeightGrowthWeight;	//max_h, meaning of this parameter?
	double const _maintenanceFactor;	//k_maint
	double const _growthLimitCoefficient;	//50 in "if growth > v_tree / 50 ["
	double const _deathThreshold;	//death.tresh, 0.004
	double _size;	//size
	double const _sizeFactor;	//size-factor

	std::size_t const _ID;
	Land & _thisLand;
	std::size_t _nearestNodeID;

	double const _iniHir;
	double const _iniRootRadius;
	double const _iniStemRadius;
	double const _iniSize;

	double iniHir();
	double iniRootRadius();
	double iniStemRadius();
	double iniSize();
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

	std::size_t findNearestNodeToTree() const;
	std::size_t findNearestNodeFromIDs(std::vector<std::size_t> nodeIDs) const;
	std::vector<std::size_t> const findNodesInRadius(
			double radius = -1.0) const;

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

	double calcCrownArea() const {
		return 2 * BettinaConstants::pi * std::pow(_crownRadius, 2);
	}

protected:
	NearestNodeTable *_crownRadiusNodeTable; //for faster node searching
	NearestNodeTable *_rootRadiusNodeTable;
//	int ab;

};

#endif /* TREE_H_ */
