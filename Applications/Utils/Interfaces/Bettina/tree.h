/*
 * tree.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef TREE_H_
#define TREE_H_

#include "GeoLib/GEOObjects.h"

#include "land.h"

class tree {
public:
	tree();
	virtual ~tree();

	void grow();

	void findNearestNodeToTree(land const &aLand);

	const GeoLib::Point& getPosition() const {
		return position;
	}

	void setPosition(const GeoLib::Point& position) {
		this->position = position;
	}

	double getBranchVolume() const {
		return _branchVolume;
	}

	void setBranchVolume(double branchVolume) {
		_branchVolume = branchVolume;
	}

	double getCableRootVolume() const {
		return _cableRootVolume;
	}

	void setCableRootVolume(double cableRootVolume) {
		_cableRootVolume = cableRootVolume;
	}

	double getCrownHeight() const {
		return _crownHeight;
	}

	void setCrownHeight(double crownHeight) {
		_crownHeight = crownHeight;
	}

	double getCrownRadius() const {
		return _crownRadius;
	}

	void setCrownRadius(double crownRadius) {
		_crownRadius = crownRadius;
	}

	double getFineRootPermeability() const {
		return _fineRootPermeability;
	}

	void setFineRootPermeability(double fineRootPermeability) {
		_fineRootPermeability = fineRootPermeability;
	}

	double getFineRootVolume() const {
		return _fineRootVolume;
	}

	void setFineRootVolume(double fineRootVolume) {
		_fineRootVolume = fineRootVolume;
	}

	double getHalfMaxHeightGrowthWeigth() const {
		return _halfMaxHeightGrowthWeigth;
	}

	void setHalfMaxHeightGrowthWeigth(double halfMaxHeightGrowthWeigth) {
		_halfMaxHeightGrowthWeigth = halfMaxHeightGrowthWeigth;
	}

	double getLateralFluxResistence() const {
		return _lateralFluxResistence;
	}

	void setLateralFluxResistence(double lateralFluxResistence) {
		_lateralFluxResistence = lateralFluxResistence;
	}

	double getLeafVolume() const {
		return _leafVolume;
	}

	void setLeafVolume(double leafVolume) {
		_leafVolume = leafVolume;
	}

	double getMaintanceFactor() const {
		return _maintanceFactor;
	}

	void setMaintanceFactor(double maintanceFactor) {
		_maintanceFactor = maintanceFactor;
	}

	double getMinimumLeafWaterPotential() const {
		return _minimumLeafWaterPotential;
	}

	void setMinimumLeafWaterPotential(double minimumLeafWaterPotential) {
		_minimumLeafWaterPotential = minimumLeafWaterPotential;
	}

	double getRadialFluxResistence() const {
		return _radialFluxResistence;
	}

	void setRadialFluxResistence(double radialFluxResistence) {
		_radialFluxResistence = radialFluxResistence;
	}

	double getRootDepth() const {
		return _rootDepth;
	}

	void setRootDepth(double rootDepth) {
		_rootDepth = rootDepth;
	}

	double getRootRadius() const {
		return _rootRadius;
	}

	void setRootRadius(double rootRadius) {
		_rootRadius = rootRadius;
	}

	double getSolarRadiation() const {
		return _solarRadiation;
	}

	void setSolarRadiation(double solarRadiation) {
		_solarRadiation = solarRadiation;
	}

	double getStemHeight() const {
		return _stemHeight;
	}

	void setStemHeight(double stemHeight) {
		_stemHeight = stemHeight;
	}

	double getStemRadius() const {
		return _stemRadius;
	}

	void setStemRadius(double stemRadius) {
		_stemRadius = stemRadius;
	}

	double getStemVolume() const {
		return _stemVolume;
	}

	void setStemVolume(double stemVolume) {
		_stemVolume = stemVolume;
	}

	double getTreeVolume() const {
		return _treeVolume;
	}

	void setTreeVolume(double treeVolume) {
		_treeVolume = treeVolume;
	}

	double getXylemConductivity() const {
		return _xylemConductivity;
	}

	void setXylemConductivity(double xylemConductivity) {
		_xylemConductivity = xylemConductivity;
	}

	double getAboveC() const {
		return _aboveGroundCompetitionCoefficient;
	}

	void setAboveC(double aboveC) {
		_aboveGroundCompetitionCoefficient = aboveC;
	}

	double getBelowC() const {
		return _belowGroundCompetitionCoefficient;
	}

	void setBelowC(double belowC) {
		_belowGroundCompetitionCoefficient = belowC;
	}

	double getCg() const {
		return cg;
	}

	void setCg(double cg) {
		this->cg = cg;
	}

	double getCountabove() const {
		return countabove;
	}

	void setCountabove(double countabove) {
		this->countabove = countabove;
	}

	double getCountbelow() const {
		return countbelow;
	}

	void setCountbelow(double countbelow) {
		this->countbelow = countbelow;
	}

	double getDeathflag() const {
		return _deathFlag;
	}

	void setDeathflag(double deathflag) {
		this->_deathFlag = deathflag;
	}

	double getDg() const {
		return dg;
	}

	void setDg(double dg) {
		this->dg = dg;
	}

	double getGrowth() const {
		return growth;
	}

	void setGrowth(double growth) {
		this->growth = growth;
	}

	double getHg() const {
		return hg;
	}

	void setHg(double hg) {
		this->hg = hg;
	}

	double getMindist() const {
		return mindist;
	}

	void setMindist(double mindist) {
		this->mindist = mindist;
	}

	double getResA() const {
		return res_a;
	}

	void setResA(double resA) {
		res_a = resA;
	}

	double getResAvail() const {
		return res_avail;
	}

	void setResAvail(double resAvail) {
		res_avail = resAvail;
	}

	double getResB() const {
		return res_b;
	}

	void setResB(double resB) {
		res_b = resB;
	}

	double getRg() const {
		return rg;
	}

	void setRg(double rg) {
		this->rg = rg;
	}

	double getStepfrac() const {
		return _stepFrac;
	}

	void setStepfrac(double stepfrac) {
		this->_stepFrac = stepfrac;
	}

	double getWa1() const {
		return wa1;
	}

	void setWa1(double wa1) {
		this->wa1 = wa1;
	}

	double getWa2() const {
		return wa2;
	}

	void setWa2(double wa2) {
		this->wa2 = wa2;
	}

	double getWb1() const {
		return wb1;
	}

	void setWb1(double wb1) {
		this->wb1 = wb1;
	}

	double getWb2() const {
		return wb2;
	}

	void setWb2(double wb2) {
		this->wb2 = wb2;
	}

	double getWinCountabove() const {
		return win_countabove;
	}

	void setWinCountabove(double winCountabove) {
		win_countabove = winCountabove;
	}

	double getWinCountbelow() const {
		return win_countbelow;
	}

	void setWinCountbelow(double winCountbelow) {
		win_countbelow = winCountbelow;
	}

	double getSize() const {
		return _size;
	}

	void setSize(double size) {
		_size = size;
	}

	unsigned int getId() const {
		return _id;
	}

	void setId(unsigned int id) {
		_id = id;
	}

protected:
	GeoLib::Point position;
	double _stemRadius;
	double _crownRadius;
	double _rootRadius;
	double _stemHeight;
	double _crownHeight;
	double _rootDepth;
	double _leafVolume;
	double _branchVolume;
	double _stemVolume;
	double _cableRootVolume;
	double _fineRootVolume;
	double _treeVolume;
	double _radialFluxResistence;	//Wasseraufnahmeẃiderstand, radialer Fluß
	double _lateralFluxResistence;	//Widerstand axialer Fluß (cable roots, Stamm, Äste)
	double res_a;
	double res_b;
	double res_avail;
	double wa1;
	double wa2;
	double wb1;
	double wb2;
	double growth;
	double hg;
	double cg;
	double rg;
	double dg;
	double _stepFrac;	// numerical helper variable
	double countabove;
	double win_countabove;
	double countbelow;
	double win_countbelow;
	double _aboveGroundCompetitionCoefficient;
	double _belowGroundCompetitionCoefficient;
	double _deathFlag;
	double mindist;
	double _fineRootPermeability;
	double _minimumLeafWaterPotential;
	double _xylemConductivity;	// = permeability?
	double _solarRadiation;
	double _halfMaxHeightGrowthWeigth;	// meaning of this parameter?
	double _maintanceFactor;

	unsigned int _id;
	std::size_t _nearestNode;

	double _size;
	double _sizeFactor;

};

#endif /* TREE_H_ */
