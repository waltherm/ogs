/**
 * \copyright
 * Copyright (c) 2012-2017, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 * \file:   createTortuosityModel.h
 *
 * Created on August 16, 2016, 1:16 PM
 */

#pragma once

#include <memory>

namespace BaseLib
{
class ConfigTree;
}

namespace MaterialLib
{
namespace PorousMedium
{
class Tortuosity;

/** Create a tortuosity model
 *  @param config  ConfigTree object has a tag of `<tortuosity>`
 */
std::unique_ptr<Tortuosity> createTortuosityModel(
    BaseLib::ConfigTree const& config);

}  // end namespace
}  // end namespace
