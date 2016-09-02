/*
 * avicennia.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef AVICENNIA_H_
#define AVICENNIA_H_

#include <Tree.h>

class Avicennia: public Tree {
public:
	Avicennia(GeoLib::Point const &point, unsigned int id, Land &aLand);
	virtual ~Avicennia();
};

#endif /* AVICENNIA_H_ */
