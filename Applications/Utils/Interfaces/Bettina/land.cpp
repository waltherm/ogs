/*
 * land.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <iostream>
#include <boost/optional.hpp>

#include "land.h"

#include "MeshLib/IO/readMeshFromFile.h"
#include "MeshLib/IO/writeMeshToFile.h"

#include "MeshGeoToolsLib/MeshNodeSearcher.h"


land::land(std::string const &fileName) {
	// TODO Auto-generated constructor stub
	readMesh(fileName);
}

land::~land() {
	// TODO Auto-generated destructor stub
}

void land::readMesh(std::string const &fileName)
{
	_mesh = MeshLib::IO::readMeshFromFile(fileName);
}

double land::getSalinityAtPoint(GeoLib::Point const point)
{
	//GeoLib::Point const point(x, y, z);
	MeshGeoToolsLib::SearchLength searchLength(_mesh->getMinEdgeLength()/5);
	MeshGeoToolsLib::MeshNodeSearcher _meshSearcher(*_mesh, searchLength);
	auto idVector(_meshSearcher.getMeshNodeIDs(point));
	auto nearestNode = idVector[0];	//TODO: get nearest point from list of points
	std::string const property ("Salinity");	//TODO: check if "Salinity" available as property
	boost::optional<MeshLib::PropertyVector<double>&> const salinities(_mesh->getProperties().getPropertyVector<double>(property));
	return (*salinities)[nearestNode];
}
