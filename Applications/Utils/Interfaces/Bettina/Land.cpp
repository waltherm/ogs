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

Land::Land(std::string const &fileName,
		std::string const &salinityPropertyString,
		std::string const &aboveGroundCompetitionString,
		std::string const &belowGroundCompetitionString) :
		_salinityPropertyString(salinityPropertyString), _aboveGroundCompetitionString(
				aboveGroundCompetitionString), _belowGroundCompetitionString(
				belowGroundCompetitionString) {
	// TODO Auto-generated constructor stub
	readMesh(fileName);
	checkPropertyExists(_salinityPropertyString);
	checkPropertyExists(_aboveGroundCompetitionString);
	checkPropertyExists(_belowGroundCompetitionString);
}

Land::~Land() {
	// TODO Auto-generated destructor stub
}

void Land::readMesh(std::string const &fileName) {
	_subsurface = MeshLib::IO::readMeshFromFile(fileName);
}

void Land::checkPropertyExists(std::string propertyString) const {
	bool propCheck(false);
	for (auto &propName : _subsurface->getProperties().getPropertyVectorNames()) {
		if (propName == propertyString)
			propCheck = true;
	}
	if (!propCheck) {
		ERR("There is no property named '%s' in the mesh. Exiting.",
				propertyString.c_str());
		std::abort();
	}
}

double Land::getSalinityAtNodeID(std::size_t nodeID) const {
	return getPropertyAtNodeID<double>(nodeID, _salinityPropertyString);
}

double Land::getAboveGroundCompetitionAtNodeID(std::size_t nodeID) const {
	return getPropertyAtNodeID<double>(nodeID, _aboveGroundCompetitionString);
}

double Land::getBelowGroundCompetitionAtNodeID(std::size_t nodeID) const {
	return getPropertyAtNodeID<double>(nodeID, _belowGroundCompetitionString);
}

void Land::resetAboveGroundCompetition() {
	resetPropertyValues(-1.0, _aboveGroundCompetitionString);
}

void Land::resetBelowGroundCompetition() {
	resetPropertyValues(0.0, _belowGroundCompetitionString);
}

void Land::setAboveGroundCompetition(double value, std::size_t nodeID) {
	setPropertyAtNodeID(value, nodeID, _aboveGroundCompetitionString);
}

void Land::setBelowGroundCompetition(double value, std::size_t nodeID) {
	setPropertyAtNodeID(value, nodeID, _belowGroundCompetitionString);
}

void Land::incrementBelowGroundCompetition(std::size_t nodeID) {
	setBelowGroundCompetition(getBelowGroundCompetitionAtNodeID(nodeID) + 1,
			nodeID);
}

void Land::invertBelowGroundCompetition() {
	// TODO calc inverse if belowGroundCompetition > 1

	boost::optional<MeshLib::PropertyVector<double>&> property(
			_subsurface->getProperties().getPropertyVector<double>(
					_belowGroundCompetitionString));

	for (auto & v : *property) {
		if (v > 1)
			v = 1 / v;
	}

}

// obsolete
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
