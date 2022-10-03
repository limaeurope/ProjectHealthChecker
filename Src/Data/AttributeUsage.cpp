#include	"AttributeUsage.hpp"
#include	"VectorImageIterator.hpp"
#include	"APIdefs_LibraryParts.h"

template <typename T>
void AddAttribute(GS::HashTable<T, int>* io_table,
	T i_idx,
	GS::HashSet <T>* io_set = nullptr,
	GS::HashSet <T>* io_todoSet = nullptr)
{
	if (!i_idx) return;
	if (io_set && io_set->Contains(i_idx))
		return;

	if (!io_table->ContainsKey(i_idx))
	{
		io_table->Put(i_idx, 1);
		if (io_set)
			io_set->Add(i_idx);
		if(io_todoSet)
			io_todoSet->Add(i_idx);
	}
	else
		(*io_table)[i_idx]++;
}

void AddAttribute(GS::HashTable<short, int>* io_table,
	API_PenOverrideType i_idx,
	GS::HashSet <short>* io_set = nullptr,
	GS::HashSet <short>* io_todoSet = nullptr
	)
{
	AddAttribute <short> (io_table, i_idx.cutFillBackgroundPen, io_set, io_todoSet);
	AddAttribute <short> (io_table, i_idx.cutFillPen, io_set, io_todoSet);
	AddAttribute <short> (io_table, i_idx.overrideCutFillBackgroundPen, io_set, io_todoSet);
	AddAttribute <short> (io_table, i_idx.overrideCutFillPen, io_set, io_todoSet);
}

void AttributeUsage::ProcessParameters(API_Element i_element, AttributeUsageSet* io_attributeUsageSet)
{
	Int32 addParNum;
	API_AddParType** addPars;
	double a, b;

	GSErrCode err = ACAPI_LibPart_GetParams(i_element.object.libInd, &a, &b, &addParNum, &addPars);

	if (err == NoError && i_element.header.hasMemo) {
		API_ElementMemo memo;
		err = ACAPI_Element_GetMemo(i_element.header.guid, &memo, APIMemoMask_AddPars);

		for (Int32 i = 0; i < addParNum; i++)
		{
			auto par = (*memo.params)[i];
			switch (par.typeID)
			{
			case APIParT_LineTyp:
				AddAttribute<API_AttributeIndex>(&ltUsageTable, (API_AttributeIndex)par.index, &io_attributeUsageSet->ltSet);

				break;
			case APIParT_Mater:
				AddAttribute<API_AttributeIndex>(&surfUsageTable, (API_AttributeIndex)par.index, &io_attributeUsageSet->surfSet);

				break;
			case APIParT_FillPat:
				AddAttribute<API_AttributeIndex>(&fillUsageTable, (API_AttributeIndex)par.index, &io_attributeUsageSet->fillSet);

				break;
			case APIParT_PenCol:
				AddAttribute<short>(&penUsageTable, (short)par.value.real, &io_attributeUsageSet->penSet);

				break;
			case APIParT_BuildingMaterial:
				AddAttribute<API_AttributeIndex>(&buildMatUsageTable, (API_AttributeIndex)par.index, &io_attributeUsageSet->buildMatSet);

				break;
			case APIParT_Profile:
				AddAttribute<API_AttributeIndex>(&profileUsageTable, (API_AttributeIndex)par.index, &io_attributeUsageSet->profSet);

				break;
			}
		}

		ACAPI_DisposeElemMemoHdls(&memo);
	}
}

AttributeUsageSet AttributeUsage::GeneralAttributeUsageByElement(API_Element i_element)
{
	AttributeUsageSet aus;
	Int32* libInd, * addParNum;
	double* a, * b;
	API_AddParType*** addPars;
	GSErrCode err;

	switch (i_element.header.typeID)
	{
	case API_WallID:
		AddAttribute<short> (&penUsageTable, i_element.wall.contPen, &aus.penSet);
		AddAttribute<short> (&penUsageTable, i_element.wall.contPen3D, &aus.penSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.wall.contLtype, &aus.ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.wall.belowViewLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex> (&buildMatUsageTable, i_element.wall.buildingMaterial, &aus.buildMatSet);
		AddAttribute<API_AttributeIndex> (&compositeUsageTable, i_element.wall.composite, &aus.compSet);
		AddAttribute(&penUsageTable, i_element.wall.penOverride, &aus.penSet);
		AddAttribute(&penUsageTable, i_element.wall.penOverride, &aus.penSet);
		AddAttribute<API_AttributeIndex> (&surfUsageTable, i_element.wall.refMat.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex> (&surfUsageTable, i_element.wall.oppMat.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex> (&surfUsageTable, i_element.wall.sidMat.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex> (&profileUsageTable, i_element.wall.profileAttr);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.wall.aboveViewLineType, &aus.ltSet);
		
		break;
	case API_ColumnID:
		AddAttribute<short> (&penUsageTable, i_element.column.corePen, &aus.penSet);
		AddAttribute(&penUsageTable, i_element.column.penOverride, &aus.penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.belowViewLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.column.belowViewLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.contLtype, &aus.ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.venLineType, &aus.ltSet);
		AddAttribute<short> (&penUsageTable, i_element.column.venLinePen, &aus.penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.coreSymbolPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.column.hiddenLineType, &aus.ltSet);
		AddAttribute<short> (&penUsageTable, i_element.column.hiddenLinePen, &aus.penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.aboveViewLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.aboveViewLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex> (&fillUsageTable, i_element.column.coverFillType, &aus.fillSet);
		AddAttribute<short> (&penUsageTable, i_element.column.coverFillForegroundPen, &aus.penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.coverFillBackgroundPen, &aus.penSet);

		break;
	case API_BeamID:
		AddAttribute<short> (&penUsageTable, i_element.column.corePen, &aus.penSet);
		AddAttribute(&penUsageTable, i_element.column.penOverride, &aus.penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.belowViewLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.belowViewLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.contLtype, &aus.ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.venLineType, &aus.ltSet);
		AddAttribute<short> (&penUsageTable, i_element.column.venLinePen, &aus.penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.coreSymbolPen, &aus.penSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.hiddenLineType, &aus.ltSet);
		AddAttribute<short> (&penUsageTable, i_element.column.hiddenLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex> (&fillUsageTable, i_element.column.coverFillType, &aus.fillSet);

		break;
	case API_CurtainWallSegmentID:
		AddAttribute<short>(&penUsageTable, i_element.cwSegment.pen, &aus.penSet);

		break;
	case API_CurtainWallFrameID:
		AddAttribute<short> (&penUsageTable, i_element.cwFrame.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.cwFrame.material, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&buildMatUsageTable, i_element.cwFrame.buildingMaterial, &aus.buildMatSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.cwFrame.cutLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.cwFrame.cutLinePen, &aus.penSet);

		break;
	case API_CurtainWallPanelID:
		AddAttribute<short>(&penUsageTable, i_element.cwPanel.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.cwPanel.outerSurfaceMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.cwPanel.innerSurfaceMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.cwPanel.cutSurfaceMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&buildMatUsageTable, i_element.cwPanel.buildingMaterial, &aus.buildMatSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.cwPanel.cutLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.cwPanel.cutLinePen, &aus.penSet);

		break;
	case API_CurtainWallJunctionID:
		AddAttribute<short>(&penUsageTable, i_element.cwJunction.pen, &aus.penSet);

		break;
	case API_CurtainWallAccessoryID:
		AddAttribute<short>(&penUsageTable, i_element.cwAccessory.pen, &aus.penSet);

		break;
	case API_CurtainWallID:
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.curtainWall.overheadLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.overheadLinePen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.uncutLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.curtainWall.uncutLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.curtainWall.frameCutLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.frameCutLinePen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.panelCutLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.curtainWall.panelCutLineType, &aus.ltSet);

		break;
	case API_ColumnSegmentID:
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.columnSegment.extrusionSurfaceMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.columnSegment.endsMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.columnSegment.venBuildingMaterial, &aus.surfSet);

		break;
	case API_BeamSegmentID:
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.leftMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.topMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.rightMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.bottomMaterial.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.endsMaterial.attributeIndex, &aus.surfSet);

		break;
	case API_DoorID:
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.door.openingBase.ltypeInd, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.door.openingBase.mat, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&fillUsageTable, i_element.door.openingBase.sectFill, &aus.fillSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.sectFillPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.sectBGPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.sectContPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.door.openingBase.cutLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.door.openingBase.aboveViewLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.aboveViewLinePen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.belowViewLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.door.openingBase.belowViewLineType, &aus.ltSet);

		ProcessParameters(i_element, &aus);

		break;
	case API_WindowID:
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.window.openingBase.ltypeInd, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.window.openingBase.mat, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&fillUsageTable, i_element.window.openingBase.sectFill, &aus.fillSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.sectFillPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.sectBGPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.sectContPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.window.openingBase.cutLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.window.openingBase.aboveViewLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.aboveViewLinePen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.belowViewLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.window.openingBase.belowViewLineType, &aus.ltSet);

		ProcessParameters(i_element, &aus);

		break;
	case API_ObjectID:
		AddAttribute<short>(&penUsageTable, i_element.object.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.object.ltypeInd, &aus.ltSet);
		AddAttribute < API_AttributeIndex>(&surfUsageTable, i_element.object.mat, &aus.surfSet);
		AddAttribute < API_AttributeIndex>(&fillUsageTable, i_element.object.sectFill, &aus.fillSet);
		AddAttribute<short>(&penUsageTable, i_element.object.sectFillPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.object.sectBGPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.object.sectContPen, &aus.penSet);

		ProcessParameters(i_element, &aus);

		break;
	case API_LampID:
		AddAttribute<short>(&penUsageTable, i_element.lamp.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.lamp.ltypeInd, &aus.ltSet);
		AddAttribute < API_AttributeIndex>(&surfUsageTable, i_element.lamp.mat, &aus.surfSet);
		AddAttribute < API_AttributeIndex>(&fillUsageTable, i_element.lamp.sectFill, &aus.fillSet);
		AddAttribute<short>(&penUsageTable, i_element.lamp.sectFillPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.lamp.sectBGPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.lamp.sectContPen, &aus.penSet);

		ProcessParameters(i_element, &aus);

		break;
	case API_SkylightID:
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.skylight.openingBase.ltypeInd, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.skylight.openingBase.mat, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&fillUsageTable, i_element.skylight.openingBase.sectFill, &aus.fillSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.sectFillPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.sectBGPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.sectContPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.skylight.openingBase.cutLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.skylight.openingBase.aboveViewLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.aboveViewLinePen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.belowViewLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.skylight.openingBase.belowViewLineType, &aus.ltSet);

		ProcessParameters(i_element, &aus);

		break;
	case API_SlabID:
		AddAttribute<short>(&penUsageTable, i_element.slab.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.slab.ltypeInd, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&buildMatUsageTable, i_element.slab.buildingMaterial, &aus.buildMatSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.slab.composite, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.slab.sectContPen, &aus.penSet);
		AddAttribute(&penUsageTable, i_element.slab.penOverride, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.slab.topMat.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.slab.sideMat.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.slab.botMat.attributeIndex, &aus.surfSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.slab.hiddenContourLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.slab.hiddenContourLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.slab.sectContLtype, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.slab.floorFillPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.slab.floorFillBGPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.slab.floorFillInd, &aus.compSet);

		break;
	case API_RoofID:
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.roof.shellBase.ltypeInd, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.buildingMaterial, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.composite, &aus.compSet);
		AddAttribute(&penUsageTable, i_element.roof.shellBase.penOverride, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.sectContLtype, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.roof.shellBase.sectContPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.roof.shellBase.floorFillPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.floorFillInd, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.roof.shellBase.floorFillBGPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.roof.shellBase.aboveViewLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.roof.shellBase.aboveViewLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.topMat.attributeIndex, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.sidMat.attributeIndex, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.botMat.attributeIndex, &aus.compSet);

		break;;
	case API_ShellID:
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.shell.shellBase.ltypeInd, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.buildingMaterial, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.composite, &aus.compSet);
		AddAttribute(&penUsageTable, i_element.shell.shellBase.penOverride, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.sectContLtype, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.sectContPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.floorFillPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.floorFillInd, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.floorFillBGPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.aboveViewLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.shell.shellBase.aboveViewLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.topMat.attributeIndex, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.sidMat.attributeIndex, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.botMat.attributeIndex, &aus.compSet);

		break;
	case API_MorphID:
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.morph.buildingMaterial, &aus.compSet);
		AddAttribute(&penUsageTable, i_element.morph.penOverride, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.morph.cutLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.cutLinePen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.uncutLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.morph.uncutLineType, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.morph.overheadLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.overheadLinePen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.morph.coverFillType, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.coverFillPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.coverFillBGPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.morph.material.attributeIndex, &aus.compSet);

		break;

	case API_MeshID:
		AddAttribute<short>(&penUsageTable, i_element.mesh.contPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.levelPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.mesh.ltypeInd, &aus.ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.buildingMaterial, &aus.compSet);
		AddAttribute(&penUsageTable, i_element.mesh.penOverride, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.sectContPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.topMat.attributeIndex, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.sideMat.attributeIndex, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.botMat.attributeIndex, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.showLines, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.skirt, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.floorFillPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.floorFillBGPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.floorFillInd, &aus.compSet);

		break;

	case API_DimensionID:
		AddAttribute<short>(&penUsageTable, i_element.dimension.linPen, &aus.penSet);

		break;

	case API_RadialDimensionID:
		AddAttribute<short>(&penUsageTable, i_element.radialDimension.linPen, &aus.penSet);

		break;
	case API_LevelDimensionID:
		AddAttribute<short>(&penUsageTable, i_element.levelDimension.pen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.levelDimension.dimForm, &aus.penSet);

		break;
	case API_AngleDimensionID:
		AddAttribute<short>(&penUsageTable, i_element.angleDimension.linPen, &aus.penSet);

		break;
	case API_TextID:
		AddAttribute<short>(&penUsageTable, i_element.text.pen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.text.font, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.text.contourPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.text.fillPen, &aus.penSet);

		break;
	case API_ZoneID:
		AddAttribute<short>(&penUsageTable, i_element.zone.pen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.zone.catInd, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.zone.material, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.zone.floorFillPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.zone.floorFillInd, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.zone.floorFillBGPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.zone.floorContLPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.zone.floorContLType, &aus.compSet);
		AddAttribute<short>(&penUsageTable, i_element.zone.relativeTopStory, &aus.penSet);

		break;

	case API_HatchID:
		AddAttribute<short>(&penUsageTable, i_element.hatch.fillBGPen, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.hatch.fillInd, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.hatch.buildingMaterial, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.hatch.ltypeInd, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.hatch.determination, &aus.penSet);

		break;
	case API_LineID:
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.line.ltypeInd, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.line.determination, &aus.penSet);

		break;

	case API_PolyLineID:
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.polyLine.ltypeInd, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.polyLine.determination, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.polyLine.drawSegmentMode, &aus.penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.polyLine.ltypeInd, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.polyLine.determination, &aus.penSet);

		break;
	case API_SplineID:
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.spline.ltypeInd, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.spline.determination, &aus.penSet);

		break;
	case API_HotspotID:
		AddAttribute<short>(&penUsageTable, i_element.hotspot.pen, &aus.penSet);

		break;
	case API_InteriorElevationID:
		AddAttribute<short>(&penUsageTable, i_element.interiorElevation.storyToShowOn, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.interiorElevation.markerPen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.interiorElevation.markerFont, &aus.penSet);

		break;
	case API_CameraID:
		AddAttribute<short>(&penUsageTable, i_element.camera.perspCam.pen, &aus.penSet);

		break;
	case API_CamSetID:
		AddAttribute<short>(&penUsageTable, i_element.camset.perspPars.pen, &aus.penSet);

		break;
	case API_DrawingID:
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.drawing.penTableIndex, &aus.compSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.drawing.borderLineType, &aus.ltSet);
		AddAttribute<short>(&penUsageTable, i_element.drawing.borderPen, &aus.penSet);

		break;
	case API_RailingID:
		AddAttribute<short>(&penUsageTable, i_element.railing.referenceLinePen, &aus.penSet);
		AddAttribute<short>(&penUsageTable, i_element.railing.contourPen, &aus.penSet);

		break;
	}

	return aus;
}

AttributeUsageSet AttributeUsage::GeneralAttributeUsageByAttribute(API_Attribute i_attribute, 
		API_AttributeDefExt i_defs, 
		API_AttrTypeID i_type)
{
	AttributeUsageSet newSet, todoSet;
	VectorImage vi;
	ConstVectorImageIterator cvii{ vi };

	switch (i_type)
	{
	case API_FilltypeID:
		break;
	case API_MaterialID:
		AddAttribute<API_AttributeIndex>(&fillUsageTable, i_attribute.material.ifill, &newSet.fillSet, &todoSet.fillSet);

		break;
	case API_BuildingMaterialID:
		AddAttribute<API_AttributeIndex>(&fillUsageTable, i_attribute.buildingMaterial.cutFill, &newSet.fillSet, &todoSet.fillSet);
		AddAttribute<short>(&penUsageTable, i_attribute.buildingMaterial.cutFillPen, &newSet.penSet);
		AddAttribute<short>(&penUsageTable, i_attribute.buildingMaterial.cutFillBackgroundPen, &newSet.penSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_attribute.buildingMaterial.cutMaterial, &newSet.surfSet, &todoSet.surfSet);

		break;
	case API_CompWallID:
		for (short j = 0; j < i_attribute.compWall.nComps; j++)
		{
			AddAttribute<API_AttributeIndex>(&buildMatUsageTable, (*i_defs.cwall_compItems)[j].buildingMaterial, &newSet.buildMatSet, &todoSet.buildMatSet);
			AddAttribute<short>(&penUsageTable, (*i_defs.cwall_compItems)[j].framePen, &newSet.penSet, &todoSet.penSet);
		}

		if (i_attribute.compWall.nComps)
			for (short j = 0; j < i_attribute.compWall.nComps + 1; j++)
			{
				AddAttribute<short>(&penUsageTable, (*i_defs.cwall_compLItems)[j].linePen, &newSet.penSet, &todoSet.penSet);
				AddAttribute<API_AttributeIndex>(&ltUsageTable, (*i_defs.cwall_compLItems)[j].ltypeInd, &newSet.ltSet, &todoSet.ltSet);
			}

		break;
	case API_ProfileID:
		vi = i_defs.profile_vectorImageItems->GetVectorImage();

		while (!cvii.IsEOI())
		{
			const Sy_HatchType* sht = cvii;

			if (sht)
			{
				HatchObject ho = vi.GetHatchObject(*sht);
				AddAttribute<short>(&penUsageTable, ho.fillBkgPen, &newSet.penSet, &todoSet.penSet);
				//TODO
			}

			cvii++;
		}

		break;
	case API_ZoneCatID:
		break;
	}

	return todoSet;
}

template <typename T>
void AttributeUsage::ProcessAttributeSet(const GS::HashSet<T>& i_set, 
		API_AttrTypeID i_type, 
		AttributeUsageSet& io_todoSet)
{
	GSErrCode			err;
	API_Attribute		attrib;
	API_AttributeDefExt	defs;

	for (T k : i_set)
	{
		BNZeroMemory(&attrib, sizeof(API_Attribute));
		BNZeroMemory(&defs, sizeof(API_AttributeDefExt));

		attrib.header.typeID = i_type;
		attrib.header.index = k;

		err = ACAPI_Attribute_Get(&attrib);
		
		if (err == NoError)
			err = ACAPI_Attribute_GetDefExt(i_type, k, &defs);

		io_todoSet.Append(GeneralAttributeUsageByAttribute(attrib, defs, i_type));

		ACAPI_DisposeAttrDefsHdlsExt(&defs);
	}
}

void AttributeUsage::GeneralAttributeUsageController()
{
	AttributeUsageSet originalSet{ this }, todoSet;
	
	do
	{
		todoSet = AttributeUsageSet{};
		
		ProcessAttributeSet<API_AttributeIndex>(originalSet.fillSet, API_FilltypeID, todoSet);
		ProcessAttributeSet<API_AttributeIndex>(originalSet.surfSet, API_MaterialID, todoSet);
		ProcessAttributeSet<API_AttributeIndex>(originalSet.buildMatSet, API_BuildingMaterialID, todoSet);
		ProcessAttributeSet<API_AttributeIndex>(originalSet.compSet, API_CompWallID, todoSet);
		originalSet = todoSet;
	} while (todoSet.GetSize());
}

void AttributeUsage::LayerUsage(API_Element i_element)
{
	if (!layerContentTable.ContainsKey(i_element.header.layer))
	{
		layerContentTable.Put(i_element.header.layer, 1);
	}
	else
		layerContentTable[i_element.header.layer]++;
}

AttributeUsage::AttributeUsage()
{
	API_Element         element;
	AttributeUsageSet	aus;

	GS::Array<API_Guid> elemList;
	GSErrCode err = ACAPI_Element_GetElemList(API_ZombieElemID, &elemList);

	for (auto e : elemList)
	{
		BNZeroMemory(&element, sizeof(API_Element));
		element.header.guid = e;
		err = ACAPI_Element_Get(&element);
		if (err) throw err;

		LayerUsage(element);
		GeneralAttributeUsageByElement(element);
	}

	GeneralAttributeUsageController();
}

void AttributeUsageSet::Append(AttributeUsageSet& i_other)
{
	for(auto i:i_other.penSet)
		penSet.Add(i);
	for(auto i:i_other.ltSet)
		ltSet.Add(i);
	for(auto i:i_other.fillSet)
		fillSet.Add(i);
	for(auto i:i_other.buildMatSet)
		buildMatSet.Add(i);
	for(auto i:i_other.compSet)
		compSet.Add(i);
	for(auto i:i_other.surfSet)
		surfSet.Add(i);
}

GS::USize AttributeUsageSet::GetSize()
{
	GS::USize resultSize{};

	resultSize += penSet.GetSize();
	resultSize += ltSet.GetSize();
	resultSize += fillSet.GetSize();
	resultSize += buildMatSet.GetSize();
	resultSize += compSet.GetSize();
	resultSize += surfSet.GetSize();

	return resultSize;
}

AttributeUsageSet::AttributeUsageSet(AttributeUsage* i_attributeUsage)
{
	for (auto k: i_attributeUsage->penUsageTable.Keys())
		penSet.Add(k);
	for (auto k: i_attributeUsage->ltUsageTable.Keys())
		ltSet.Add(k);
	for (auto k: i_attributeUsage->fillUsageTable.Keys())
		fillSet.Add(k);
	for (auto k: i_attributeUsage->buildMatUsageTable.Keys())
		buildMatSet.Add(k);
	for (auto k: i_attributeUsage->compositeUsageTable.Keys())
		compSet.Add(k);
	for (auto k: i_attributeUsage->surfUsageTable.Keys())
		surfSet.Add(k);
}

