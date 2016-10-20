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

	// ini points and vertices
	vtkSmartPointer<vtkPoints> treePoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> treeVertices =
			vtkSmartPointer<vtkCellArray>::New();

	// ini all data arrays
	vtkSmartPointer<vtkDoubleArray> treeData_stemRadius = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_crownRadius = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_rootRadius = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_stemHeight = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_crownHeight = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_rootDepth = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_leafVolume = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_branchVolume = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_stemVolume = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_cableRootVolume = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_fineRootVolume = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_treeVolume = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_radialFluxResistence =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_lateralFluxResistence =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_aboveGroundResources =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_belowGroundResources =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_availableResources =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_crownRadiusGrowthWeight =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_stemHeightGrowthWeight =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_fineRootGrowthWeight =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_stemRadiusGrowthWeight =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_growth = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_stemHeightGrowth = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_crownRadiusGrowth =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_rootRadiusGrowth = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_stemRadiusGrowth = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_aboveGroundCompetitionCoefficient =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_belowGroundCompetitionCoefficient =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkIntArray> treeData_deathFlag = vtkSmartPointer<
			vtkIntArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_fineRootPermeability =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_minimumLeafWaterPotential =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_xylemConductivity =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_halfMaxHeightGrowthWeigth =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_maintanceFactor = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_growthLimitCoefficient =
			vtkSmartPointer<vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_deathThreshold = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_size = vtkSmartPointer<
			vtkDoubleArray>::New();
	vtkSmartPointer<vtkDoubleArray> treeData_sizeFactor = vtkSmartPointer<
			vtkDoubleArray>::New();

	vtkSmartPointer<vtkDoubleArray> treeData_aboveGroundCompetitionWins = vtkSmartPointer<
				vtkDoubleArray>::New();

	//give names to arrays
	treeData_stemRadius->SetName("stemRadius");
	treeData_crownRadius->SetName("crownRadius");
	treeData_rootRadius->SetName("rootRadius");
	treeData_stemHeight->SetName("stemHeight");
	treeData_crownHeight->SetName("crownHeight");
	treeData_rootDepth->SetName("rootDepth");
	treeData_leafVolume->SetName("leafVolume");
	treeData_branchVolume->SetName("branchVolume");
	treeData_stemVolume->SetName("stemVolume");
	treeData_cableRootVolume->SetName("cableRootVolume");
	treeData_fineRootVolume->SetName("fineRootVolume");
	treeData_treeVolume->SetName("treeVolume");
	treeData_radialFluxResistence->SetName("radialFluxResistence");
	treeData_lateralFluxResistence->SetName("lateralFluxResistence");
	treeData_aboveGroundResources->SetName("aboveGroundResources");
	treeData_belowGroundResources->SetName("belowGroundResources");
	treeData_availableResources->SetName("availableResources");
	treeData_crownRadiusGrowthWeight->SetName("crownRadiusGrowthWeight");
	treeData_stemHeightGrowthWeight->SetName("stemHeightGrowthWeight");
	treeData_fineRootGrowthWeight->SetName("fineRootGrowthWeight");
	treeData_stemRadiusGrowthWeight->SetName("stemRadiusGrowthWeight");
	treeData_growth->SetName("growth");
	treeData_stemHeightGrowth->SetName("stemHeightGrowth");
	treeData_crownRadiusGrowth->SetName("crownRadiusGrowth");
	treeData_rootRadiusGrowth->SetName("rootRadiusGrowth");
	treeData_stemRadiusGrowth->SetName("stemRadiusGrowth");
	treeData_aboveGroundCompetitionCoefficient->SetName(
			"aboveGroundCompetitionCoefficient");
	treeData_belowGroundCompetitionCoefficient->SetName(
			"belowGroundCompetitionCoefficient");
	treeData_deathFlag->SetName("deathFlag");
	treeData_fineRootPermeability->SetName("fineRootPermeability");
	treeData_minimumLeafWaterPotential->SetName("minimumLeafWaterPotential");
	treeData_xylemConductivity->SetName("xylemConductivity");
	treeData_halfMaxHeightGrowthWeigth->SetName("halfMaxHeightGrowthWeigth");
	treeData_maintanceFactor->SetName("maintanceFactor");
	treeData_growthLimitCoefficient->SetName("growthLimitCoefficient");
	treeData_deathThreshold->SetName("deathThreshold");
	treeData_size->SetName("size");
	treeData_sizeFactor->SetName("sizeFactor");

	treeData_aboveGroundCompetitionWins->SetName("aboveGroundCompetitionWins");

	std::size_t idCounter(0);
	for (auto &thisTree : aFlora.getAliveTrees()) {

		//insert points per tree and values
		treePoints->InsertNextPoint(thisTree->getPosition()[0],
				thisTree->getPosition()[1], thisTree->getPosition()[2]);
		treeVertices->InsertNextCell(1);
		treeVertices->InsertCellPoint(idCounter); //give point id of neuronPoints

		treeData_stemRadius->InsertNextValue(thisTree->getStemRadius());
		treeData_crownRadius->InsertNextValue(thisTree->getCrownRadius());
		treeData_rootRadius->InsertNextValue(thisTree->getRootRadius());
		treeData_stemHeight->InsertNextValue(thisTree->getStemHeight());
		treeData_crownHeight->InsertNextValue(thisTree->getCrownHeight());
		treeData_rootDepth->InsertNextValue(thisTree->getRootDepth());
		treeData_leafVolume->InsertNextValue(thisTree->getLeafVolume());
		treeData_branchVolume->InsertNextValue(thisTree->getBranchVolume());
		treeData_stemVolume->InsertNextValue(thisTree->getStemVolume());
		treeData_cableRootVolume->InsertNextValue(
				thisTree->getCableRootVolume());
		treeData_fineRootVolume->InsertNextValue(thisTree->getFineRootVolume());
		treeData_treeVolume->InsertNextValue(thisTree->getTreeVolume());
		treeData_radialFluxResistence->InsertNextValue(
				thisTree->getRadialFluxResistence());
		treeData_lateralFluxResistence->InsertNextValue(
				thisTree->getLateralFluxResistence());
		treeData_aboveGroundResources->InsertNextValue(
				thisTree->getAboveGroundResources());
		treeData_belowGroundResources->InsertNextValue(
				thisTree->getBelowGroundResources());
		treeData_availableResources->InsertNextValue(
				thisTree->getAvailableResources());
		treeData_crownRadiusGrowthWeight->InsertNextValue(
				thisTree->getCrownRadiusGrowthWeight());
		treeData_stemHeightGrowthWeight->InsertNextValue(
				thisTree->getStemHeightGrowthWeight());
		treeData_fineRootGrowthWeight->InsertNextValue(
				thisTree->getFineRootGrowthWeight());
		treeData_stemRadiusGrowthWeight->InsertNextValue(
				thisTree->getStemRadiusGrowthWeight());
		treeData_growth->InsertNextValue(thisTree->getGrowth());
		treeData_stemHeightGrowth->InsertNextValue(
				thisTree->getStemHeightGrowth());
		treeData_crownRadiusGrowth->InsertNextValue(
				thisTree->getCrownRadiusGrowth());
		treeData_rootRadiusGrowth->InsertNextValue(
				thisTree->getRootRadiusGrowth());
		treeData_stemRadiusGrowth->InsertNextValue(
				thisTree->getStemRadiusGrowth());
		treeData_aboveGroundCompetitionCoefficient->InsertNextValue(
				thisTree->getAboveGroundCompetitionCoefficient());
		treeData_belowGroundCompetitionCoefficient->InsertNextValue(
				thisTree->getBelowGroundCompetitionCoefficient());
		treeData_deathFlag->InsertNextValue(thisTree->getDeathFlag());
		treeData_fineRootPermeability->InsertNextValue(
				thisTree->getFineRootPermeability());
		treeData_minimumLeafWaterPotential->InsertNextValue(
				thisTree->getMinimumLeafWaterPotential());
		treeData_xylemConductivity->InsertNextValue(
				thisTree->getXylemConductivity());
		treeData_halfMaxHeightGrowthWeigth->InsertNextValue(
				thisTree->getHalfMaxHeightGrowthWeigth());
		treeData_maintanceFactor->InsertNextValue(
				thisTree->getMaintanceFactor());
		treeData_growthLimitCoefficient->InsertNextValue(
				thisTree->getGrowthLimitCoefficient());
		treeData_deathThreshold->InsertNextValue(thisTree->getDeathThreshold());
		treeData_size->InsertNextValue(thisTree->getSize());
		treeData_sizeFactor->InsertNextValue(thisTree->getSizeFactor());

		treeData_aboveGroundCompetitionWins->InsertNextValue(thisTree->getWinCountabove());

		idCounter++;
	}

	//ini grid and add arrays
	vtkSmartPointer<vtkUnstructuredGrid> treeGrid = vtkSmartPointer<
			vtkUnstructuredGrid>::New();
	treeGrid->SetPoints(treePoints);
	treeGrid->SetCells(VTK_VERTEX, treeVertices);
	treeGrid->GetCellData()->AddArray(treeData_stemRadius);
	treeGrid->GetCellData()->AddArray(treeData_crownRadius);
	treeGrid->GetCellData()->AddArray(treeData_rootRadius);
	treeGrid->GetCellData()->AddArray(treeData_stemHeight);
	treeGrid->GetCellData()->AddArray(treeData_crownHeight);
	treeGrid->GetCellData()->AddArray(treeData_rootDepth);
	treeGrid->GetCellData()->AddArray(treeData_leafVolume);
	treeGrid->GetCellData()->AddArray(treeData_branchVolume);
	treeGrid->GetCellData()->AddArray(treeData_stemVolume);
	treeGrid->GetCellData()->AddArray(treeData_cableRootVolume);
	treeGrid->GetCellData()->AddArray(treeData_fineRootVolume);
	treeGrid->GetCellData()->AddArray(treeData_treeVolume);
	treeGrid->GetCellData()->AddArray(treeData_radialFluxResistence);
	treeGrid->GetCellData()->AddArray(treeData_lateralFluxResistence);
	treeGrid->GetCellData()->AddArray(treeData_aboveGroundResources);
	treeGrid->GetCellData()->AddArray(treeData_belowGroundResources);
	treeGrid->GetCellData()->AddArray(treeData_availableResources);
	treeGrid->GetCellData()->AddArray(treeData_crownRadiusGrowthWeight);
	treeGrid->GetCellData()->AddArray(treeData_stemHeightGrowthWeight);
	treeGrid->GetCellData()->AddArray(treeData_fineRootGrowthWeight);
	treeGrid->GetCellData()->AddArray(treeData_stemRadiusGrowthWeight);
	treeGrid->GetCellData()->AddArray(treeData_growth);
	treeGrid->GetCellData()->AddArray(treeData_stemHeightGrowth);
	treeGrid->GetCellData()->AddArray(treeData_crownRadiusGrowth);
	treeGrid->GetCellData()->AddArray(treeData_rootRadiusGrowth);
	treeGrid->GetCellData()->AddArray(treeData_stemRadiusGrowth);
	treeGrid->GetCellData()->AddArray(
			treeData_aboveGroundCompetitionCoefficient);
	treeGrid->GetCellData()->AddArray(
			treeData_belowGroundCompetitionCoefficient);
	treeGrid->GetCellData()->AddArray(treeData_deathFlag);
	treeGrid->GetCellData()->AddArray(treeData_fineRootPermeability);
	treeGrid->GetCellData()->AddArray(treeData_minimumLeafWaterPotential);
	treeGrid->GetCellData()->AddArray(treeData_xylemConductivity);
	treeGrid->GetCellData()->AddArray(treeData_halfMaxHeightGrowthWeigth);
	treeGrid->GetCellData()->AddArray(treeData_maintanceFactor);
	treeGrid->GetCellData()->AddArray(treeData_growthLimitCoefficient);
	treeGrid->GetCellData()->AddArray(treeData_deathThreshold);
	treeGrid->GetCellData()->AddArray(treeData_size);
	treeGrid->GetCellData()->AddArray(treeData_sizeFactor);

	treeGrid->GetCellData()->AddArray(treeData_aboveGroundCompetitionWins);

	//ini writer and write
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
