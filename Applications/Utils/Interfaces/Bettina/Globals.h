/*
 * globals.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <boost/math/constants/constants.hpp>

namespace BettinaConstants {

	double const pi (boost::math::constants::pi<double>());

	double const gravityConstant(9.81);

	// what do the following parameters mean and do?
	double const k_geom(1);
	double const k_rel(1);
	double const k_grow(0.1);
	double const Qr0(0);
	double const sigmo_slope_hg(0.15);
	double const sigmo_slope(0.02);
	double const seedSpreadFactor(1.2);

	double const growthLimitCoefficient(50);
	double const deathTreshhold(0.0001);
	double const solarRadiation(0.038);

	double const aviSizeFactor(4);

	double const searchRadiusIncrement(2);

	double const oneOverSqrtTwo(std::pow(2, -0.5));

//	double const salRand;
//	double const rrd1;
//	double const rrd2;
//	double const rrd3;
//	double const rsd1;
//	double const rsd2;
//	double const rsd3;
//	double const hsd1;
//	double const hsd2;
//	double const hsd3;
//	double const rcd1;
//	double const rcd2;
//	double const rcd3;

	//unsigned int _countDead;

};


class Globals {
public:
	Globals();
	virtual ~Globals();

	void calculateClarkEvansIndex();


	double getClarkEvans() const {
		return _clarkEvans;
	}

	void setClarkEvans(double clarkEvans) {
		this->_clarkEvans = clarkEvans;
	}

	double getMce() const {
		return _mce;
	}

	void setMce(double mce) {
		this->_mce = mce;
	}

private:
	double _clarkEvans;
	double _mce;
	double _currentTimeStep;

};

#endif /* GLOBALS_H_ */
