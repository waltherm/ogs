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


class land {
public:
	land(std::string const &fileName);
	virtual ~land();

private:
	void readMesh(std::string const &fileName);

	std::unique_ptr<MeshLib::Mesh> _mesh;
};

#endif /* LAND_H_ */
