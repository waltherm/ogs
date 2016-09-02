/*
 * rhizophora.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <Rhizophora.h>

Rhizophora::Rhizophora(GeoLib::Point const &point, unsigned int id,
		Land &aLand) :
		Tree(point, id, aLand, 0.0001, 0.004, 0.004, 0.3, 0.04, -7860, 1.48,
				0.1, 0.28) {		//TODO check ini variables for rhizo
	// TODO Auto-generated constructor stub
}

Rhizophora::~Rhizophora() {
	// TODO Auto-generated destructor stub
}

