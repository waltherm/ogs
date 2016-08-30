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
	Land(std::string const &fileName);
	virtual ~Land();

	double getSalinityAtPoint(GeoLib::Point const &point) const;
	double getSalinityAtNodeID(std::size_t nodeID) const;
	void updateSalinityAtPoint();

	const MeshLib::Mesh* getSubsurface() const {
		return _subsurface;
	}

private:
	void readMesh(std::string const &fileName);

	MeshLib::Mesh *_subsurface;

};

#endif /* LAND_H_ */
