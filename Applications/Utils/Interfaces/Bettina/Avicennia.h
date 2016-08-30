/*
 * avicennia.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef APPLICATIONS_UTILS_INTERFACES_BETTINA_AVICENNIA_H_
#define APPLICATIONS_UTILS_INTERFACES_BETTINA_AVICENNIA_H_

#include <Tree.h>

class Avicennia: public Tree {
public:
	Avicennia(GeoLib::Point const &point, unsigned int id, Land const &aLand);
	virtual ~Avicennia();
};

#endif /* APPLICATIONS_UTILS_INTERFACES_BETTINA_AVICENNIA_H_ */
