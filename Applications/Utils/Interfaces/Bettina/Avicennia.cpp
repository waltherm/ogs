/*
 * avicennia.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include <Avicennia.h>

Avicennia::Avicennia(GeoLib::Point const &point, unsigned int id,
		Land const &aLand) :
		Tree(point, id, aLand, 0.0001, 0.004, 0.004, 0.3, 0.04, -7860, 1.48,
				0.1, 0.28) {
	// TODO Auto-generated constructor stub
}

Avicennia::~Avicennia() {
	// TODO Auto-generated destructor stub
}

