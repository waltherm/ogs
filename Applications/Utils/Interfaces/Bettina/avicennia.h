/*
 * avicennia.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef AVICENNIA_H_
#define AVICENNIA_H_

#include "tree.h"

class avicennia: public tree {
public:
	avicennia(GeoLib::Point const &point, double salinity, unsigned int id, land const &aLand);
	virtual ~avicennia();
};

#endif /* AVICENNIA_H_ */
