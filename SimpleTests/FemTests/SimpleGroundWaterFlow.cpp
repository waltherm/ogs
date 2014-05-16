/**
 * \file
 * \author
 * \date   2013-10-08
 * \brief  Implementation of simple ground water flow process
 *
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#include <cstdlib>

#ifdef OGS_USE_EIGEN
#include <Eigen/Eigen>
#endif

// AssemblerLib
#include "AssemblerLib/SerialDenseSetup.h"
#include "AssemblerLib/VectorMatrixAssembler.h"

// ThirdParty/logog
#include "logog/include/logog.hpp"

// ThirdParty/tclap
#include "ThirdParty/tclap/CmdLine.h"

// BaseLib
#include "BaseLib/LogogSimpleFormatter.h"
#include "BaseLib/FileTools.h"
#include "Configure.h"

// FileIO
#include "XmlIO/Boost/BoostXmlCndInterface.h"
#include "XmlIO/Boost/BoostXmlGmlInterface.h"
#include "readMeshFromFile.h"

// GeoLib
#include "GEOObjects.h"
#include "GeoObject.h"

// MathLib
#include "MathLib/LinAlg/Solvers/GaussAlgorithm.h"
#include "MathLib/TemplateWeightedPoint.h"

// MeshGeoToolsLib
#include "MeshNodeSearcher.h"
#include "MeshNodesToPoints.h"

// MeshLib
#include "MeshLib/Mesh.h"
#include "MeshLib/MeshSubsets.h"
#include "MeshLib/Elements/Quad.h"

// NumLib
#include "NumLib/Fem/Integration/IntegrationGaussRegular.h"
#include "NumLib/Fem/CoordinatesMapping/ShapeMatrices.h"
#include "NumLib/Fem/ShapeFunction/ShapeQuad4.h"
#include "NumLib/Fem/FiniteElement/C0IsoparametricElements.h"

// OGS
#include "BoundaryCondition.h"
#include "ProjectData.h"

template <typename ElemType>
class LocalGWAssembler
{

public:
    typedef Eigen::Matrix<double, ElemType::NPOINTS, ElemType::NPOINTS, Eigen::RowMajor> NodalMatrixType;
    typedef Eigen::Matrix<double, ElemType::NPOINTS, 1> NodalVectorType;
    typedef Eigen::Matrix<double, ElemType::DIM, ElemType::NPOINTS, Eigen::RowMajor> DimNodalMatrixType;
    typedef Eigen::Matrix<double, ElemType::DIM, ElemType::DIM, Eigen::RowMajor> DimMatrixType;

public:
	LocalGWAssembler() :
			_m(ElemType::NPOINTS, ElemType::NPOINTS),
			_shape(ElemType::DIM, ElemType::NPOINTS),
			_integration_method(2)
	{}

	void operator()(const MeshLib::Element& e, NodalMatrixType &localA,
			NodalVectorType & /*rhs*/)
	{
		_m.setZero();

		// init FeQUAD4
		_fe_quad4.setMeshElement(*static_cast<const MeshLib::Quad*>(&e));

		for (std::size_t ip(0); ip < _integration_method.getNPoints(); ip++) { // ip == number of gauss point
			std::cout << ip << " (r,s) = " << _integration_method.getWeightedPoint(ip) << " w = " << _integration_method.getWeightedPoint(ip).getWeight() << "\n";

			_shape.setZero();
			MathLib::WeightedPoint2D const& wp = _integration_method.getWeightedPoint(ip);
			_fe_quad4.computeShapeFunctions(wp.getCoords(), _shape);
			std::cout << "dNdx = " << _shape.dNdx << " J = " << _shape.detJ << std::endl;
			std::cout << "m_" << ip << " = " << _m << std::endl;
			_m += _shape.dNdx.transpose() * _shape.dNdx * _shape.detJ * wp.getWeight();
		}

		std::cout << "ip = " << _m << std::endl;

		for (std::size_t i = 0; i < _integration_method.getNPoints(); i++)
			for (std::size_t j = 0; j < _integration_method.getNPoints(); j++)
				localA(i, j) = _m(i, j);
	}

private:
	NodalMatrixType _m;
	typedef typename NumLib::FeQUAD4<NodalVectorType, DimNodalMatrixType, DimMatrixType>::type FeQuad4;
	typename FeQuad4::ShapeMatricesType _shape;
	typename FeQuad4::IntegrationMethod _integration_method;

	FeQuad4 _fe_quad4;
};


int main(int argc, char *argv[])
{
	// logog
	LOGOG_INITIALIZE();
	BaseLib::LogogSimpleFormatter *custom_format(new BaseLib::LogogSimpleFormatter);
	logog::Cout *logog_cout(new logog::Cout);
	logog_cout->SetFormatter(*custom_format);

	// tclap
	TCLAP::CmdLine cmd(
			"Simple ground water flow test, reading mesh (only 2d quad elements), geometry and bc and simulate ground water flow",
			' ', "0.1");

	TCLAP::ValueArg<std::string> mesh_arg("m", "mesh", "file name of the mesh", true, "", "string");
	cmd.add(mesh_arg);

	TCLAP::ValueArg<std::string> geometry_arg("g", "geometry", "file name of the geometry", true,
			"", "string");
	cmd.add(geometry_arg);

	TCLAP::ValueArg<std::string> bc_arg("", "boundary_condition",
			"file name of the boundary condition", true, "", "string");
	cmd.add(bc_arg);

	cmd.parse(argc, argv);

	ProjectData project_data;

	// *** read geometry
	{
		FileIO::BoostXmlGmlInterface geo_io(project_data);
		geo_io.readFile(geometry_arg.getValue());
	}
	std::string unique_name;

	// *** read mesh
	project_data.addMesh(FileIO::readMeshFromFile(mesh_arg.getValue()));

	// *** read boundary conditions
	FileIO::BoostXmlCndInterface xml_io(project_data);
	xml_io.readFile(bc_arg.getValue());

	std::vector<FEMCondition*> bcs(
			project_data.getConditions(FiniteElement::GROUNDWATER_FLOW, unique_name,
					FEMCondition::BOUNDARY_CONDITION));

	std::vector < std::size_t > bc_mesh_node_ids;
	const std::string mesh_name(BaseLib::extractBaseNameWithoutExtension(mesh_arg.getValue()));
	MeshGeoToolsLib::MeshNodeSearcher searcher(*project_data.getMesh(mesh_name));
	for (auto it(bcs.cbegin()); it != bcs.cend(); it++) {
		// fetch geometry obj from condition obj
		GeoLib::GeoObject const* geom_obj((*it)->getGeoObj());
		if (dynamic_cast<GeoLib::Point const*>(geom_obj) != nullptr) {
			GeoLib::Point const& pnt(*dynamic_cast<GeoLib::Point const*>(geom_obj));
			bc_mesh_node_ids.push_back(searcher.getMeshNodeIDForPoint(pnt));
		} else {
			if (dynamic_cast<GeoLib::Polyline const*>(geom_obj) != nullptr) {
				GeoLib::Polyline const& ply(*dynamic_cast<GeoLib::Polyline const*>(geom_obj));
				std::vector<std::size_t> const& ids(searcher.getMeshNodeIDsAlongPolyline(ply));
				bc_mesh_node_ids.insert(bc_mesh_node_ids.end(), ids.cbegin(), ids.cend());
			}
		}
	}

	//--------------------------------------------------------------------------
	// Prepare mesh items where data are assigned
	//--------------------------------------------------------------------------
	MeshLib::Mesh const& mesh(*project_data.getMesh(mesh_name));
	const MeshLib::MeshSubset mesh_items_all_nodes(mesh, mesh.getNodes());

	//-------------------------------------------------------------------------
	// Allocate a (global) coefficient matrix, RHS and solution vectors
	//-------------------------------------------------------------------------
	// define a mesh item composition in a vector
	std::vector<MeshLib::MeshSubsets*> vec_comp_dis;
	vec_comp_dis.push_back(new MeshLib::MeshSubsets(&mesh_items_all_nodes));
	AssemblerLib::MeshComponentMap vec1_composition(vec_comp_dis,
			AssemblerLib::ComponentOrder::BY_COMPONENT);

	//--------------------------------------------------------------------------
	// Choose implementation type
	//--------------------------------------------------------------------------
	typedef AssemblerLib::SerialDenseSetup GlobalSetup;
	const GlobalSetup global_setup;

	// allocate a vector and matrix
	typedef GlobalSetup::VectorType GlobalVector;
	typedef GlobalSetup::MatrixType GlobalMatrix;
	std::unique_ptr < GlobalMatrix > A(global_setup.createMatrix(vec1_composition));
	A->setZero();
	std::unique_ptr < GlobalVector > rhs(global_setup.createVector(vec1_composition));
	std::unique_ptr < GlobalVector > x(global_setup.createVector(vec1_composition));

	//--------------------------------------------------------------------------
	// Construct a linear system
	//--------------------------------------------------------------------------
	// create a mapping table from element nodes to entries in the linear system
	std::vector<MeshLib::Element*> const& all_eles = mesh.getElements();

	std::vector < std::vector<std::size_t> > map_ele_nodes2vec_entries;
	map_ele_nodes2vec_entries.reserve(all_eles.size());
	for (auto e = all_eles.cbegin(); e != all_eles.cend(); ++e) {
		std::size_t const nnodes = (*e)->getNNodes();
		std::size_t const mesh_id = mesh.getID();
		std::vector<MeshLib::Location> vec_items;
		vec_items.reserve(nnodes);
		for (std::size_t j = 0; j < nnodes; j++)
			vec_items.emplace_back(mesh_id, MeshLib::MeshItemType::Node, (*e)->getNode(j)->getID());

		map_ele_nodes2vec_entries.push_back(
				vec1_composition.getGlobalIndices<AssemblerLib::ComponentOrder::BY_COMPONENT>(
						vec_items));
	}

	//
	// Local and global assemblers.
	//
	LocalGWAssembler<NumLib::ShapeQuad4> local_gw_assembler;
	typedef typename LocalGWAssembler<NumLib::ShapeQuad4>::NodalMatrixType LocalMatrix;
	typedef typename LocalGWAssembler<NumLib::ShapeQuad4>::NodalVectorType LocalVector;

	typedef AssemblerLib::VectorMatrixAssembler<
			GlobalMatrix,
			GlobalVector,
			MeshLib::Element,
			LocalGWAssembler<NumLib::ShapeQuad4>,
			LocalMatrix,
			LocalVector > GlobalAssembler;

	GlobalAssembler global_assembler(*A.get(), *rhs.get(), local_gw_assembler,
			AssemblerLib::LocalToGlobalIndexMap(map_ele_nodes2vec_entries));

	// Call global assembler for each mesh element.
	global_setup.execute(global_assembler, mesh.getElements());

	//--------------------------------------------------------------------------
	// solve x=A^-1 rhs
	//--------------------------------------------------------------------------
	std::cout << "A=\n";
	for (std::size_t i = 0; i < 30; i++) {
		for (std::size_t j = 0; j < 30; j++)
			std::cout << (*A)(i, j) << " ";
		std::cout << std::endl;
	}

	MathLib::GaussAlgorithm<GlobalMatrix, GlobalVector> ls(*A);
	ls.solve(*rhs, *x);

	if (x->size() > 1000) {
		std::ofstream out("results.txt");
		for (std::size_t i = 0; i < x->size(); i++) {
			out << (*x)[i] << " ";
		}
		out << std::endl;
		out.close();
	} else {
		for (std::size_t i = 0; i < x->size(); i++) {
			std::cout << (*x)[i] << " ";
		}
	}

	delete custom_format;
	delete logog_cout;
	LOGOG_SHUTDOWN();

	return 0;
}
