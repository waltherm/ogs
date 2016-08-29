/*
 * land.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include "land.h"

#include "MeshLib/IO/readMeshFromFile.h"
#include "MeshLib/IO/writeMeshToFile.h"


land::land(std::string const &fileName) {
	// TODO Auto-generated constructor stub
	readMesh(fileName);
}

land::~land() {
	// TODO Auto-generated destructor stub
}

void land::readMesh(std::string const &fileName)
{
	_mesh.reset(MeshLib::IO::readMeshFromFile(fileName));
}
