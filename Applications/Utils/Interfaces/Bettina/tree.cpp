/*
 * tree.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include "MeshGeoToolsLib/MeshNodeSearcher.h"

#include "tree.h"


tree::tree():
	_stepFrac(0),
	_aboveGroundCompetitionCoefficient(1),
	_belowGroundCompetitionCoefficient(1),
	_deathFlag(0),
	_sizeFactor(4)
{
	// TODO Auto-generated constructor stub

}

tree::~tree() {
	// TODO Auto-generated destructor stub
}

void tree::grow()
{

}

void tree::findNearestNodeToTree(land const &aLand)
{
	MeshGeoToolsLib::SearchLength searchLength(aLand.getMesh()->getMinEdgeLength());
	MeshGeoToolsLib::MeshNodeSearcher _meshSearcher(*aLand.getMesh(), searchLength);
	auto idVector(_meshSearcher.getMeshNodeIDs(this->getPosition()));
	if (idVector.size() == 0)
	{
		ERR("No nodes found near tree no. %u.", this->getId());
	}
	else
	{
		if (idVector.size() == 1)
			_nearestNode = idVector[0];
		else
		{
			_nearestNode = idVector[0];	//TODO: get nearest point from list of points
		}
	}
}
