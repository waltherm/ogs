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

private:
	void readMesh(std::string const &fileName);

	MeshLib::Mesh *_mesh;
};

#endif /* LAND_H_ */
