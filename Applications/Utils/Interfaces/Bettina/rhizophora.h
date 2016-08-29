/*
 * rhizophora.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef RHIZOPHORA_H_
#define RHIZOPHORA_H_

#include "tree.h"

class rhizophora: public tree {
public:
	rhizophora(GeoLib::Point point, double salinity);
	virtual ~rhizophora();
};

#endif /* RHIZOPHORA_H_ */
