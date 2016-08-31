/*
 * land.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <iostream>
#include <boost/optional.hpp>
#include <Land.h>

#include "MeshLib/IO/readMeshFromFile.h"

#include "MeshGeoToolsLib/MeshNodeSearcher.h"

#include "Flora.h"

Land::Land(std::string const &fileName)
{
	// TODO Auto-generated constructor stub
	readMesh(fileName);
}

Land::~Land() {
	// TODO Auto-generated destructor stub
}

void Land::readMesh(std::string const &fileName) {
	_subsurface = MeshLib::IO::readMeshFromFile(fileName);
}

double Land::getSalinityAtPoint(GeoLib::Point const &point) const {
	//GeoLib::Point const point(x, y, z);
	MeshGeoToolsLib::SearchLength searchLength(
			_subsurface->getMinEdgeLength() / 5);
	MeshGeoToolsLib::MeshNodeSearcher _meshSearcher(*_subsurface, searchLength);
	auto idVector(_meshSearcher.getMeshNodeIDs(point));
	auto nearestNode = idVector[0];	//TODO: get nearest point from list of points
	std::string const property("Salinity");	//TODO: check if "Salinity" available as property
	boost::optional<MeshLib::PropertyVector<double>&> const salinities(
			_subsurface->getProperties().getPropertyVector<double>(property));
	return (*salinities)[nearestNode];
}

double Land::getSalinityAtNodeID(std::size_t nodeID) const {
	std::string const property("Salinity");	//TODO: check if "Salinity" available as property
	boost::optional<MeshLib::PropertyVector<double>&> const salinities(
			_subsurface->getProperties().getPropertyVector<double>(property));
	return (*salinities)[nodeID];
}

void Land::updateSalinityAtPoint() {
	// TODO
}
