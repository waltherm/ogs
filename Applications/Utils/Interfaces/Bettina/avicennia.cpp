/*
 * avicennia.cpp
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#include "avicennia.h"

avicennia::avicennia(GeoLib::Point const &point, unsigned int id,
		land const &aLand) :
		tree(point, id, aLand, 0.0001, 0.004, 0.004, 0.3, 0.04, -7860, 1.48,
				0.1, 0.28) {
	// TODO Auto-generated constructor stub
}

avicennia::~avicennia() {
	// TODO Auto-generated destructor stub
}

