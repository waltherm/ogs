/**
 * \file
 *
 * \copyright
 * Copyright (c) 2012-2018, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#include "Pipe.h"
#include "BaseLib/ConfigTree.h"

namespace ProcessLib
{
namespace HeatTransportBHE
{
namespace BHE
{
Pipe createPipe(BaseLib::ConfigTree const& config)
{
    //! \ogs_file_param{prj__processes__process__HEAT_TRANSPORT_BHE__borehole_heat_exchangers__borehole_heat_exchanger__pipes__inlet__radius}
    const double radius = config.getConfigParameter<double>("radius");
    const double wall_thickness =
        //! \ogs_file_param{prj__processes__process__HEAT_TRANSPORT_BHE__borehole_heat_exchangers__borehole_heat_exchanger__pipes__inlet__wall_thickness}
        config.getConfigParameter<double>("wall_thickness");
    const double wall_thermal_conductivity =
        //! \ogs_file_param{prj__processes__process__HEAT_TRANSPORT_BHE__borehole_heat_exchangers__borehole_heat_exchanger__pipes__inlet__wall_thermal_conductivity}
        config.getConfigParameter<double>("wall_thermal_conductivity");
    return {radius, wall_thickness, wall_thermal_conductivity};
}
}  // namespace BHE
}  // namespace HeatTransportBHE
}  // namespace ProcessLib
