/*
 * rhizophora.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef RHIZOPHORA_H_
#define RHIZOPHORA_H_

#include <Tree.h>

class Rhizophora: public Tree {
public:
	Rhizophora(GeoLib::Point const &point, unsigned int id, Land const &aLand);
	virtual ~Rhizophora();
};

#endif /* RHIZOPHORA_H_ */
