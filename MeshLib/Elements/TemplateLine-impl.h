/**
 * \file
 * \author Thomas Fischer
 * \date   Sep 27, 2012
 * \brief  Implementation of the TemplateLine class.
 *
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

namespace MeshLib
{
template<unsigned NNODES, CellType CELLLINETYPE, typename TRAITS>
TemplateLine<NNODES,CELLLINETYPE,TRAITS>::TemplateLine(
	std::array<Node*, NNODES> const& nodes,
	unsigned value)
	: Edge(value, id)
{
	this->_nodes = new Node*[NNODES];
	std::copy(nodes.begin(), nodes.end(), this->_nodes);

	_neighbors = new Element*[2];
	std::fill_n(_neighbors, 2, nullptr);

	this->_length = this->computeVolume();
}

template<unsigned NNODES, CellType CELLLINETYPE, typename TRAITS>
TemplateLine<NNODES,CELLLINETYPE,TRAITS>::TemplateLine(Node* nodes[NNODES], unsigned value) :
	Edge(value, id)
{
	this->_nodes = nodes;
	this->_length = this->computeVolume();
}

template <unsigned NNODES, CellType CELLLINETYPE, typename TRAITS>
TemplateLine<NNODES,CELLLINETYPE,TRAITS>::TemplateLine(const TemplateLine<NNODES,CELLLINETYPE,TRAITS> &line) :
	Edge(line.getValue(), line.getID())
{
	this->_nodes = new Node*[NNODES];
	for (unsigned k(0); k<NNODES; k++)
		this->_nodes[k] = line._nodes[k];
	this->_length = line.getLength();
}

template <unsigned NNODES, CellType CELLLINETYPE, typename TRAITS>
TemplateLine<NNODES,CELLLINETYPE,TRAITS>::~TemplateLine()
{}

template <unsigned NNODES, CellType CELLLINETYPE, typename TRAITS>
ElementErrorCode TemplateLine<NNODES,CELLLINETYPE,TRAITS>::validate() const
{ 
	ElementErrorCode error_code;
	error_code[ElementErrorFlag::ZeroVolume] = this->hasZeroVolume();
	return error_code;
}

} // namespace MeshLib

