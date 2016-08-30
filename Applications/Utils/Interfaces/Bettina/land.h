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


class land {
public:
	land(std::string const &fileName);
	virtual ~land();

	double getSalinityAtPoint(GeoLib::Point const point);
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

#endif /* LAND_H_ */
