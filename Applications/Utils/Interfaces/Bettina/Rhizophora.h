/*
 * rhizophora.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef APPLICATIONS_UTILS_INTERFACES_BETTINA_RHIZOPHORA_H_
#define APPLICATIONS_UTILS_INTERFACES_BETTINA_RHIZOPHORA_H_

#include <Tree.h>

class Rhizophora: public Tree {
public:
	Rhizophora(GeoLib::Point const &point, unsigned int id, Land const &aLand);
	virtual ~Rhizophora();
};

#endif /* APPLICATIONS_UTILS_INTERFACES_BETTINA_RHIZOPHORA_H_ */
