/*
 * land.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef LAND_H_
#define LAND_H_

#include <string>
#include <memory>

#include "MeshLib/Mesh.h"
#include "GeoLib/GEOObjects.h"

class Flora;

class Land {
public:
	Land(std::string const &fileName, std::string const &salinityPropertyString,
			std::string const &aboveGroundCompetitionString,
			std::string const &belowGroundCompetitionString);
	virtual ~Land();

	double getSalinityAtPoint(GeoLib::Point const &point) const;
	double getSalinityAtNodeID(std::size_t nodeID) const;

	void resetAboveGroundCompetition();
	void resetBelowGroundCompetition();

	const MeshLib::Mesh* getSubsurface() const {
		return _subsurface;
	}

private:
	void readMesh(std::string const &fileName);
	void checkPropertyExists(std::string propertyString) const;

	template<typename PROP_VAL_TYPE>
	PROP_VAL_TYPE getPropertyAtNodeID(std::size_t nodeID,
			std::string propertyString) const {

		std::string const propertyName(propertyString);
		boost::optional<MeshLib::PropertyVector<PROP_VAL_TYPE>&> const property(
				_subsurface->getProperties().getPropertyVector<double>(
						propertyName));
		return (*property)[nodeID];

	}

	template<typename PROP_VAL_TYPE>
	void setPropertyAtNodeID(PROP_VAL_TYPE value, size_t nodeID,
			std::string propertyString) {

		std::string const propertyName(propertyString);
		boost::optional<MeshLib::PropertyVector<PROP_VAL_TYPE>&> const property(
				_subsurface->getProperties().getPropertyVector<double>(
						propertyName));

		if (property.getNumberOfTuples() < nodeID) {
			ERR("There is no node with ID '%i' in the mesh. Exiting.", nodeID);
			std::abort();
		}
		property[nodeID] = value;
	}

	template<typename PROP_VAL_TYPE>
	void resetPropertyValues(PROP_VAL_TYPE resetValue,
			std::string propertyString) {

		std::string const propertyName(propertyString);
		boost::optional<MeshLib::PropertyVector<PROP_VAL_TYPE>&> const property(
				_subsurface->getProperties().getPropertyVector<double>(
						propertyName));

		for (auto & thisProperty : *property)
			thisProperty = resetValue;

	}

	std::string _salinityPropertyString;
	std::string _aboveGroundCompetitionString;
	std::string _belowGroundCompetitionString;
	MeshLib::Mesh *_subsurface;

};

#endif /* LAND_H_ */
