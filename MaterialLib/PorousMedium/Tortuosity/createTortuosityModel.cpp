/**
 * \copyright
 * Copyright (c) 2012-2017, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 * \file:   createTortuosityModel.cpp
 *
 * Created on August 16, 2016, 1:16 PM
 */

#include "createTortuosityModel.h"

#include "BaseLib/Error.h"
#include "BaseLib/ConfigTree.h"

#include "Tortuosity.h"
#include "ConstantTortuosity.h"

namespace MaterialLib
{
namespace PorousMedium
{
std::unique_ptr<Tortuosity> createTortuosityModel(BaseLib::ConfigTree const& config)
{
    //! \ogs_file_param{material__porous_medium__tortuosity__type}
    auto const type = config.getConfigParameter<std::string>("type");

    if (type == "Constant")
        return std::unique_ptr<Tortuosity>(
            //! \ogs_file_param{material__porous_medium__tortuosity__Constant__value}
            new ConstantTortuosity(config.getConfigParameter<double>("value")));
    else
    {
        OGS_FATAL("The tortuosity type %s is unavailable.\n",
                  "The available type is \n\tConstant.", type.data());
    }
}

}  // end namespace
}  // end namespace
