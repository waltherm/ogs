/*
 * Output.cpp
 *
 *  Created on: Aug 31, 2016
 *      Author: waltherm
 */

#include <Output.h>

#include <string>

#include <vtkSmartPointer.h>
//#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>

#include "MeshLib/IO/writeMeshToFile.h"

#include "Flora.h"

Output::Output(std::string baseFileName) :
		_baseFileName(baseFileName), _floraOutFileName(
				_baseFileName + "_flora_"), _landOutFileName(
				_baseFileName + "_land_"), _floraFileEnding(".vtu"), _landFileEnding(
				".vtu") {
	// TODO Auto-generated constructor stub

}

Output::~Output() {
	// TODO Auto-generated destructor stub
}

void Output::writeFlora(Flora const & aFlora, std::size_t currentTimeStep) {

	vtkSmartPointer<vtkPoints> treePoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> treeVertices =
			vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeDataStemHeight = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkIntArray> treeDataDeathFlag =
			vtkSmartPointer<vtkIntArray>::New();
	treeDataStemHeight->SetName("StemHeight");
	treeDataDeathFlag->SetName("DeathFlag");

	std::size_t idCounter(0);
	for (auto &thisTree : aFlora.getAliveTrees()) {

		treePoints->InsertNextPoint(thisTree->getPosition()[0],
				thisTree->getPosition()[1], thisTree->getPosition()[2]);
		treeVertices->InsertNextCell(1);
		treeVertices->InsertCellPoint(idCounter); //give point id of neuronPoints
		treeDataStemHeight->InsertNextValue(thisTree->getStemHeight());
		treeDataDeathFlag->InsertNextValue(thisTree->getDeathFlag());
		idCounter++;
	}

	vtkSmartPointer<vtkUnstructuredGrid> treeGrid = vtkSmartPointer<
			vtkUnstructuredGrid>::New();
	treeGrid->SetPoints(treePoints);
	treeGrid->SetCells(VTK_VERTEX, treeVertices);
	treeGrid->GetCellData()->AddArray(treeDataStemHeight);
	treeGrid->GetCellData()->AddArray(treeDataDeathFlag);

	vtkSmartPointer<vtkXMLUnstructuredGridWriter> treeWriter = vtkSmartPointer<
			vtkXMLUnstructuredGridWriter>::New();
	std::string currentOutfile(
			_floraOutFileName + std::to_string(currentTimeStep)
					+ _floraFileEnding);

	treeWriter->SetFileName(currentOutfile.c_str());
	treeWriter->AddInputDataObject(treeGrid);
	treeWriter->Write();

}

void Output::writeLand(const MeshLib::Mesh * subsurface,
		std::size_t currentTimeStep) {

	std::string currentOutfile(
			_landOutFileName + std::to_string(currentTimeStep)
					+ _landFileEnding);
	MeshLib::IO::writeMeshToFile(*subsurface, currentOutfile);

}
