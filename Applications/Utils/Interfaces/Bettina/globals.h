/*
 * globals.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

class globals {
public:
	globals();
	virtual ~globals();

	double _clarkEvans;
	double _mce;
	double _salRand;
	double _rrd1;
	double _rrd2;
	double _rrd3;
	double _rsd1;
	double _rsd2;
	double _rsd3;
	double _hsd1;
	double _hsd2;
	double _hsd3;
	double _rcd1;
	double _rcd2;
	double _rcd3;

	unsigned int _countDead;

	double _k_geom;	// what does this do?

private:
	int _timeStep;

	void calculateClarkEvansIndex();
};

#endif /* GLOBALS_H_ */
