/*
 * land.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef APPLICATIONS_UTILS_INTERFACES_BETTINA_LAND_H_
#define APPLICATIONS_UTILS_INTERFACES_BETTINA_LAND_H_

#include <string>
#include <memory>

#include "MeshLib/Mesh.h"
#include "GeoLib/GEOObjects.h"


class Land {
public:
	Land(std::string const &fileName);
	virtual ~Land();

	double getSalinityAtPoint(GeoLib::Point const &point) const;
	double getSalinityAtNodeID(std::size_t nodeID) const;
	void updateSalinityAtPoint();

	const MeshLib::Mesh* getMesh() const {
		return _mesh;
	}

	void setMesh(MeshLib::Mesh* const mesh) {
		_mesh = mesh;
	}

private:
	void readMesh(std::string const &fileName);

	MeshLib::Mesh *_mesh;

};

#endif /* APPLICATIONS_UTILS_INTERFACES_BETTINA_LAND_H_ */
