/*
 * globals.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include "globals.h"

globals::globals():
	_mce(1),
	_k_geom(1),
	_timeStep(0)
{
	// TODO Auto-generated constructor stub

}

globals::~globals() {
	// TODO Auto-generated destructor stub
}

void globals::calculateClarkEvansIndex()
{
	//TODO only do, if tree number > 1
}
