/**
 * \copyright
 * Copyright (c) 2012-2017, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 * \file:   LinearConcentrationDependentDensity.h
 * \author: waltherm
 *
 * Created on March 09, 2017, 07:04 AM
 */

#pragma once

#include <string>

#include "BaseLib/ConfigTree.h"

#include "MaterialLib/Fluid/FluidProperty.h"

namespace MaterialLib
{
namespace Fluid
{
/// Linear concentration dependent density model.
class LinearConcentrationDependentDensity final : public FluidProperty
{
public:
    /**
     * @param rho0  \f$ \rho_0 \f$
     * @param C0    \f$ T_0 \f$
     * @param beta  \f$ \beta \f$
     */
    explicit LinearConcentrationDependentDensity(const double rho0, double C0,
                                                 const double beta)
        : _rho0(rho0), _concentration0(C0), _beta(beta)
    {
    }

    /// Get model name.
    std::string getName() const override
    {
        return "Linear concentration dependent density";
    }

    /// Get density value.
    /// \param var_vals Variable values in an array. The order of its elements
    ///                 is given in enum class PropertyVariableType.
    double getValue(const ArrayType& var_vals) const override
    {
        const double C = var_vals[static_cast<int>(PropertyVariableType::C)];
        return _rho0 * (1 + _beta * (C - _concentration0));
    }

    /// Get the partial differential of the density with respect to concentration.
    /// \param var_vals  Variable values  in an array. The order of its elements
    ///                   is given in enum class PropertyVariableType.
    /// \param var       Variable type.
    double getdValue(const ArrayType& var_vals,
                     const PropertyVariableType var) const override
    {
        (void)var_vals;
        if (var != PropertyVariableType::C)
            return 0.0;
        return _rho0 * _beta;
    }

private:
    const double _rho0;          ///<  Reference density.
    const double _concentration0;  ///<  Reference concentration.
    const double _beta;          ///<  Parameter.
};

}  // end namespace
}  // end namespace
