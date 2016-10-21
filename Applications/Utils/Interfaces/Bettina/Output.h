/*
 * Output.h
 *
 *  Created on: Aug 31, 2016
 *      Author: waltherm
 */

#ifndef APPLICATIONS_UTILS_INTERFACES_BETTINA_OUTPUT_H_
#define APPLICATIONS_UTILS_INTERFACES_BETTINA_OUTPUT_H_

#include <string>

#include "MeshLib/Mesh.h"

class Land;
class Flora;


class Output {
public:
	Output(std::string baseFileName);
	virtual ~Output();

	void writeFlora(Flora const & aFlora, std::size_t currentTimeStep);
	void writeLand(Land &aLand, std::size_t currentTimeStep);

private:

	void collectFloraOutputData(Flora const & aFlora);

	std::string _baseFileName;
	std::string _floraOutFileName;
	std::string _landOutFileName;

	const std::string _floraFileEnding;
	const std::string _landFileEnding;
};


#endif /* APPLICATIONS_UTILS_INTERFACES_BETTINA_OUTPUT_H_ */
