/**
 * \copyright
 * Copyright (c) 2012-2015, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#ifndef PROCESS_LIB_NEUMANN_BC_H_
#define PROCESS_LIB_NEUMANN_BC_H_

#include <functional>
#include <memory>
#include <vector>

#include "NumLib/Fem/FiniteElement/TemplateIsoparametric.h"
#include "NumLib/Fem/ShapeMatrixPolicy.h"

namespace ProcessLib
{

template <typename GlobalSetup>
class NeumannBC
{
public:
public:

    NeumannBC(
        GlobalSetup const& global_setup,
        unsigned const integration_order,
        std::vector<MeshLib::Element*> elements,
        std::vector<double> values,
        double const scaling_value,
        AssemblerLib::LocalToGlobalIndexMap const& local_to_global_index_map,
        MeshLib::MeshSubset const& mesh_subset_all_nodes
        )
        : _values(values),
          _scaling_value(scaling_value),
          _global_setup(global_setup),
          _integration_order(integration_order)
    {
        // deep copy of elements, because these might be destroyed before.
        std::transform(elements.cbegin(), elements.cend(),
                std::back_inserter(_elements),
                std::mem_fn(&MeshLib::Element::clone));
        std::vector<MeshLib::Node*> nodes = MeshLib::selectNodes(_elements);

        _mesh_subset_all_nodes =
            mesh_subset_all_nodes.getIntersectionByNodes(nodes);
        _all_mesh_subsets.push_back(new MeshLib::MeshSubsets(_mesh_subset_all_nodes));

        _local_to_global_index_map.reset(
            local_to_global_index_map.deriveBoundaryConstrainedMap(
                _all_mesh_subsets, _elements));
    }

    ~NeumannBC()
    {
        //for (auto p : _local_assemblers)
        //    delete p;

        for (auto p : _elements)
            delete p;
    }

    void
    initialize(
        typename GlobalSetup::MatrixType& A,
        typename GlobalSetup::VectorType& rhs)
    {
        // Shape matrices initializer
        using LocalDataInitializer = AssemblerLib::LocalDataInitializer<
            LocalNeumannBcAsmDataInterface,
            LocalNeumannBcAsmData,
            typename GlobalSetup::MatrixType,
            typename GlobalSetup::VectorType>;

        LocalDataInitializer initializer;

        using LocalAssemblerBuilder =
            AssemblerLib::LocalAssemblerBuilder<
                MeshLib::Element,
                LocalDataInitializer>;

        // Populate the vector of local assemblers.
        _local_assemblers.resize(_elements.size());
        LocalAssemblerBuilder local_asm_builder(
            initializer, *_local_to_global_index_map);

        DBUG("Calling local Neumann assembler builder for neumann boundary elements.");
        _global_setup.execute(
                local_asm_builder,
                _elements,
                _local_assemblers,
                [this](double const*) { return this->_values.front(); },
                _integration_order);

        DBUG("Create global assembler.");
        _global_assembler.reset(
            new GlobalAssembler(A, rhs, *_local_to_global_index_map));
    }

    void
    integrate()
    {
        _global_setup.execute(*_global_assembler, _local_assemblers);
    }

private:
    std::vector<MeshLib::Element*> _elements;
    std::vector<double> _values;
    double const _scaling_value;

    MeshLib::MeshSubset const* _mesh_subset_all_nodes = nullptr;
    std::vector<MeshLib::MeshSubsets*> _all_mesh_subsets;

    std::unique_ptr<AssemblerLib::LocalToGlobalIndexMap> _local_to_global_index_map;

    GlobalSetup const& _global_setup;
    unsigned const _integration_order;

    using GlobalAssembler =
        AssemblerLib::VectorMatrixAssembler<
            typename GlobalSetup::MatrixType,
            typename GlobalSetup::VectorType>;

    std::unique_ptr<GlobalAssembler> _global_assembler;

    using LocalAssembler = LocalNeumannBcAsmDataInterface<
        typename GlobalSetup::MatrixType, typename GlobalSetup::VectorType>;

    std::vector<LocalAssembler*> _local_assemblers;

};

}   // namespace ProcessLib

#endif  // PROCESS_LIB_NEUMANN_BC_H_
