/**
 * @file
 * @date Aug 9, 2010
 * @brief
 *
 * @copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/LICENSE.txt
 */

#ifndef MESHNODESALONGPOLYLINE_H_
#define MESHNODESALONGPOLYLINE_H_

#include <vector>

// GeoLib
#include "Polyline.h"

// MeshLib
#include "Node.h"

namespace MeshGeoToolsLib
{
/**
 * This class computes the ids of the mesh nodes along a polyline.
 *
 * The mesh nodes are sorted as follow:
 * [ ids of sorted nodes according to their distance to the starting point of ply ]
 */
class MeshNodesAlongPolyline
{
public:
	/**
	 * Constructor of object, that search mesh nodes along a
	 * GeoLib::Polyline polyline within a given search radius. So the polyline
	 * is something like a tube.
	 * @param mesh_nodes Nodes the search will be performed on.
	 * @param ply Along the GeoLib::Polyline ply the mesh nodes are searched.
	 * @param epsilon_radius Search / tube radius
	 */
	MeshNodesAlongPolyline(std::vector<MeshLib::Node*> const& mesh_nodes,
			GeoLib::Polyline const& ply, double epsilon_radius);
	/**
	 * Access the vector of mesh node ids.
	 * @return The vector of mesh node ids calculated in the constructor
	 */
	std::vector<std::size_t> const& getNodeIDs () const;
	/**
	 * Deploying this method the user can get access to the underlying
	 * GeoLib::Polyline.
	 * @return the underlying GeoLib::Polyline
	 */
	GeoLib::Polyline const& getPolyline () const;
	std::vector<double> const & getDistOfProjNodeFromPlyStart() const;

private:
	GeoLib::Polyline const& _ply;
	std::vector<std::size_t> _msh_node_ids;
	std::vector<double> _dist_of_proj_node_from_ply_start;
};
} // end namespace MeshGeoToolsLib

#endif /* MESHNODESALONGPOLYLINE_H_ */
