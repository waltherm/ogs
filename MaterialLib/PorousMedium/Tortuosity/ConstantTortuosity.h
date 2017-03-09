/**
 * \copyright
 * Copyright (c) 2012-2017, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 * \file   ConstantTortuosity.h
 *
 * Created on August 16, 2016, 1:03 PM
 */

#pragma once

#include "Tortuosity.h"

namespace MaterialLib
{
namespace PorousMedium
{
class ConstantTortuosity final : public Tortuosity
{
public:
    explicit ConstantTortuosity(const double value) : _value(value) {}

    /// Get model name.
    std::string getName() const override { return "Constant tortuosity"; }

    /**
     *  Get property value.
     *  @param variable    A variable with any double type value.
     */
    double getValue(const double variable) const override
    {
        (void)variable;
        return _value;
    }

private:
    const double _value;
};

}  // end of namespace
}  // end of namespace
