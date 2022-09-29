#include	"AttributeUsage.hpp"

template <typename T>
void AddAttribute(GS::HashTable<T, int>* io_table,
	T i_idx,
	GS::HashSet <T>* io_set = nullptr,
	bool i_doAddZero = false)
{
	if (!i_doAddZero && !i_idx) return;
	if (io_set && io_set->Contains(i_idx))
		return;

	if (!io_table->ContainsKey(i_idx))
	{
		io_table->Put(i_idx, 1);
		if (io_set)
			io_set->Add(i_idx);
	}
	else
		(*io_table)[i_idx]++;
}

void AddAttribute(GS::HashTable<short, int>* io_table,
	API_PenOverrideType i_idx,
	GS::HashSet <short>* io_set = nullptr,
	bool i_doAddZero = false)
{
	AddAttribute <short> (io_table, i_idx.cutFillBackgroundPen, io_set, i_doAddZero);
	AddAttribute <short> (io_table, i_idx.cutFillPen, io_set, i_doAddZero);
	AddAttribute <short> (io_table, i_idx.overrideCutFillBackgroundPen, io_set, i_doAddZero);
	AddAttribute <short> (io_table, i_idx.overrideCutFillPen, io_set, i_doAddZero);
}

void AttributeUsage::GeneralAttributeUsage(API_Element i_element)
{
	GS::HashSet <short> penSet{};
	GS::HashSet <API_AttributeIndex> ltSet{};
	GS::HashSet <API_AttributeIndex> fillSet{};
	GS::HashSet <API_AttributeIndex> buildMatSet{};
	GS::HashSet <API_AttributeIndex> compSet{};
	GS::HashSet <API_AttributeIndex> surfSet{};

	switch (i_element.header.typeID)
	{
	case API_WallID:
		AddAttribute<short> (&penUsageTable, i_element.wall.contPen, &penSet);
		AddAttribute<short> (&penUsageTable, i_element.wall.contPen3D, &penSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.wall.contLtype, &ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.wall.belowViewLineType, &ltSet);
		AddAttribute<API_AttributeIndex> (&buildMatUsageTable, i_element.wall.buildingMaterial, &buildMatSet);
		AddAttribute<API_AttributeIndex> (&compositeUsageTable, i_element.wall.composite, &compSet);
		AddAttribute(&penUsageTable, i_element.wall.penOverride, &penSet);
		AddAttribute(&penUsageTable, i_element.wall.penOverride, &penSet);
		AddAttribute<API_AttributeIndex> (&surfUsageTable, i_element.wall.refMat.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex> (&surfUsageTable, i_element.wall.oppMat.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex> (&surfUsageTable, i_element.wall.sidMat.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex> (&profileUsageTable, i_element.wall.profileAttr);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.wall.aboveViewLineType, &ltSet);
		
		break;
	case API_ColumnID:
		AddAttribute<short> (&penUsageTable, i_element.column.corePen, &penSet);
		AddAttribute(&penUsageTable, i_element.column.penOverride, &penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.belowViewLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.column.belowViewLineType, &ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.contLtype, &ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.venLineType, &ltSet);
		AddAttribute<short> (&penUsageTable, i_element.column.venLinePen, &penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.coreSymbolPen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.column.hiddenLineType, &ltSet);
		AddAttribute<short> (&penUsageTable, i_element.column.hiddenLinePen, &penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.aboveViewLinePen, &penSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.aboveViewLineType, &ltSet);
		AddAttribute<API_AttributeIndex> (&fillUsageTable, i_element.column.coverFillType, &fillSet);
		AddAttribute<short> (&penUsageTable, i_element.column.coverFillForegroundPen, &penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.coverFillBackgroundPen, &penSet);

		break;
	case API_BeamID:
		AddAttribute<short> (&penUsageTable, i_element.column.corePen, &penSet);
		AddAttribute(&penUsageTable, i_element.column.penOverride, &penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.belowViewLinePen, &penSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.belowViewLineType, &ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.contLtype, &ltSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.venLineType, &ltSet);
		AddAttribute<short> (&penUsageTable, i_element.column.venLinePen, &penSet);
		AddAttribute<short> (&penUsageTable, i_element.column.coreSymbolPen, &penSet);
		AddAttribute<API_AttributeIndex> (&ltUsageTable, i_element.column.hiddenLineType, &ltSet);
		AddAttribute<short> (&penUsageTable, i_element.column.hiddenLinePen, &penSet);
		AddAttribute<API_AttributeIndex> (&fillUsageTable, i_element.column.coverFillType, &fillSet);

		break;
	case API_CurtainWallSegmentID:
		AddAttribute<short>(&penUsageTable, i_element.cwSegment.pen, &penSet);

		break;
	case API_CurtainWallFrameID:
		AddAttribute<short> (&penUsageTable, i_element.cwFrame.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.cwFrame.material, &surfSet);
		AddAttribute<API_AttributeIndex>(&buildMatUsageTable, i_element.cwFrame.buildingMaterial, &buildMatSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.cwFrame.cutLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.cwFrame.cutLinePen, &penSet);

		break;
	case API_CurtainWallPanelID:
		AddAttribute<short>(&penUsageTable, i_element.cwPanel.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.cwPanel.outerSurfaceMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.cwPanel.innerSurfaceMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.cwPanel.cutSurfaceMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&buildMatUsageTable, i_element.cwPanel.buildingMaterial, &buildMatSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.cwPanel.cutLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.cwPanel.cutLinePen, &penSet);

		break;
	case API_CurtainWallJunctionID:
		AddAttribute<short>(&penUsageTable, i_element.cwJunction.pen, &penSet);

		break;
	case API_CurtainWallAccessoryID:
		AddAttribute<short>(&penUsageTable, i_element.cwAccessory.pen, &penSet);

		break;
	case API_CurtainWallID:
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.curtainWall.overheadLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.overheadLinePen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.uncutLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.curtainWall.uncutLineType, &ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.curtainWall.frameCutLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.frameCutLinePen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.curtainWall.panelCutLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.curtainWall.panelCutLineType, &ltSet);

		break;
	case API_ColumnSegmentID:
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.columnSegment.extrusionSurfaceMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.columnSegment.endsMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.columnSegment.venBuildingMaterial, &surfSet);

		break;
	case API_BeamSegmentID:
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.leftMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.topMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.rightMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.bottomMaterial.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.beamSegment.endsMaterial.attributeIndex, &surfSet);

		break;
	case API_DoorID:
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.door.openingBase.ltypeInd, &ltSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.door.openingBase.mat, &surfSet);
		AddAttribute<API_AttributeIndex>(&fillUsageTable, i_element.door.openingBase.sectFill, &fillSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.sectFillPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.sectBGPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.sectContPen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.door.openingBase.cutLineType, &ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.door.openingBase.aboveViewLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.aboveViewLinePen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.door.openingBase.belowViewLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.door.openingBase.belowViewLineType, &ltSet);

		break;
	case API_WindowID:
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.window.openingBase.ltypeInd, &ltSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.window.openingBase.mat, &surfSet);
		AddAttribute<API_AttributeIndex>(&fillUsageTable, i_element.window.openingBase.sectFill, &fillSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.sectFillPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.sectBGPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.sectContPen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.window.openingBase.cutLineType, &ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.window.openingBase.aboveViewLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.aboveViewLinePen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.window.openingBase.belowViewLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.window.openingBase.belowViewLineType, &ltSet);

		break;
	case API_ObjectID:
		AddAttribute<short>(&penUsageTable, i_element.object.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.object.ltypeInd, &ltSet);
		AddAttribute < API_AttributeIndex>(&surfUsageTable, i_element.object.mat, &surfSet);
		AddAttribute < API_AttributeIndex>(&fillUsageTable, i_element.object.sectFill, &fillSet);
		AddAttribute<short>(&penUsageTable, i_element.object.sectFillPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.object.sectBGPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.object.sectContPen, &penSet);

		break;
	case API_LampID:
		AddAttribute<short>(&penUsageTable, i_element.lamp.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.lamp.ltypeInd, &ltSet);
		AddAttribute < API_AttributeIndex>(&surfUsageTable, i_element.lamp.mat, &surfSet);
		AddAttribute < API_AttributeIndex>(&fillUsageTable, i_element.lamp.sectFill, &fillSet);
		AddAttribute<short>(&penUsageTable, i_element.lamp.sectFillPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.lamp.sectBGPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.lamp.sectContPen, &penSet);

		break;
	case API_SkylightID:
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.skylight.openingBase.ltypeInd, &ltSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.skylight.openingBase.mat, &surfSet);
		AddAttribute<API_AttributeIndex>(&fillUsageTable, i_element.skylight.openingBase.sectFill, &fillSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.sectFillPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.sectBGPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.sectContPen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.skylight.openingBase.cutLineType, &ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.skylight.openingBase.aboveViewLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.aboveViewLinePen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.skylight.openingBase.belowViewLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.skylight.openingBase.belowViewLineType, &ltSet);

		break;
	case API_SlabID:
		AddAttribute<short>(&penUsageTable, i_element.slab.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.slab.ltypeInd, &ltSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.slab.buildingMaterial, &surfSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.slab.composite, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.slab.sectContPen, &penSet);
		AddAttribute(&penUsageTable, i_element.slab.penOverride, &penSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.slab.topMat.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.slab.sideMat.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&surfUsageTable, i_element.slab.botMat.attributeIndex, &surfSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.slab.hiddenContourLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.slab.hiddenContourLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.slab.sectContLtype, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.slab.floorFillPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.slab.floorFillBGPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.slab.floorFillInd, &compSet);

		break;
	case API_RoofID:
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.roof.shellBase.ltypeInd, &ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.buildingMaterial, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.composite, &compSet);
		AddAttribute(&penUsageTable, i_element.roof.shellBase.penOverride, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.sectContLtype, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.roof.shellBase.sectContPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.roof.shellBase.floorFillPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.floorFillInd, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.roof.shellBase.floorFillBGPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.roof.shellBase.aboveViewLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.roof.shellBase.aboveViewLineType, &ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.topMat.attributeIndex, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.sidMat.attributeIndex, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.roof.shellBase.botMat.attributeIndex, &compSet);

		break;;
	case API_ShellID:
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.shell.shellBase.ltypeInd, &ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.buildingMaterial, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.composite, &compSet);
		AddAttribute(&penUsageTable, i_element.shell.shellBase.penOverride, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.sectContLtype, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.sectContPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.floorFillPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.floorFillInd, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.floorFillBGPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.shell.shellBase.aboveViewLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.shell.shellBase.aboveViewLineType, &ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.topMat.attributeIndex, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.sidMat.attributeIndex, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.shell.shellBase.botMat.attributeIndex, &compSet);

		break;
	case API_MorphID:
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.morph.buildingMaterial, &compSet);
		AddAttribute(&penUsageTable, i_element.morph.penOverride, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.morph.cutLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.cutLinePen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.uncutLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.morph.uncutLineType, &ltSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.morph.overheadLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.overheadLinePen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.morph.coverFillType, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.coverFillPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.morph.coverFillBGPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.morph.material.attributeIndex, &compSet);

		break;

	case API_MeshID:
		AddAttribute<short>(&penUsageTable, i_element.mesh.contPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.levelPen, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.mesh.ltypeInd, &ltSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.buildingMaterial, &compSet);
		AddAttribute(&penUsageTable, i_element.mesh.penOverride, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.sectContPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.topMat.attributeIndex, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.sideMat.attributeIndex, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.botMat.attributeIndex, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.showLines, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.skirt, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.floorFillPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.mesh.floorFillBGPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.mesh.floorFillInd, &compSet);

		break;

	case API_DimensionID:
		AddAttribute<short>(&penUsageTable, i_element.dimension.linPen, &penSet);

		break;

	case API_RadialDimensionID:
		AddAttribute<short>(&penUsageTable, i_element.radialDimension.linPen, &penSet);

		break;
	case API_LevelDimensionID:
		AddAttribute<short>(&penUsageTable, i_element.levelDimension.pen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.levelDimension.dimForm, &penSet);

		break;
	case API_AngleDimensionID:
		AddAttribute<short>(&penUsageTable, i_element.angleDimension.linPen, &penSet);

		break;
	case API_TextID:
		AddAttribute<short>(&penUsageTable, i_element.text.pen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.text.font, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.text.contourPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.text.fillPen, &penSet);

		break;
	case API_ZoneID:
		AddAttribute<short>(&penUsageTable, i_element.zone.pen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.zone.catInd, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.zone.material, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.zone.floorFillPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.zone.floorFillInd, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.zone.floorFillBGPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.zone.floorContLPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.zone.floorContLType, &compSet);
		AddAttribute<short>(&penUsageTable, i_element.zone.relativeTopStory, &penSet);

		break;

	case API_HatchID:
		AddAttribute<short>(&penUsageTable, i_element.hatch.fillBGPen, &penSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.hatch.fillInd, &compSet);
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.hatch.buildingMaterial, &compSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.hatch.ltypeInd, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.hatch.determination, &penSet);

		break;
	case API_LineID:
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.line.ltypeInd, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.line.determination, &penSet);

		break;

	case API_PolyLineID:
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.polyLine.ltypeInd, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.polyLine.determination, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.polyLine.drawSegmentMode, &penSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.polyLine.ltypeInd, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.polyLine.determination, &penSet);

		break;
	case API_SplineID:
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.spline.ltypeInd, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.spline.determination, &penSet);

		break;
	case API_HotspotID:
		AddAttribute<short>(&penUsageTable, i_element.hotspot.pen, &penSet);

		break;
	case API_InteriorElevationID:
		AddAttribute<short>(&penUsageTable, i_element.interiorElevation.storyToShowOn, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.interiorElevation.markerPen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.interiorElevation.markerFont, &penSet);

		break;
	case API_CameraID:
		AddAttribute<short>(&penUsageTable, i_element.camera.perspCam.pen, &penSet);

		break;
	case API_CamSetID:
		AddAttribute<short>(&penUsageTable, i_element.camset.perspPars.pen, &penSet);

		break;
	case API_DrawingID:
		AddAttribute<API_AttributeIndex>(&compositeUsageTable, i_element.drawing.penTableIndex, &compSet);
		AddAttribute<API_AttributeIndex>(&ltUsageTable, i_element.drawing.borderLineType, &ltSet);
		AddAttribute<short>(&penUsageTable, i_element.drawing.borderPen, &penSet);

		break;
	case API_RailingID:
		AddAttribute<short>(&penUsageTable, i_element.railing.referenceLinePen, &penSet);
		AddAttribute<short>(&penUsageTable, i_element.railing.contourPen, &penSet);

		break;
	}
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

	GS::Array<API_Guid> elemList;
	GSErrCode err = ACAPI_Element_GetElemList(API_ZombieElemID, &elemList);

	for (auto e : elemList)
	{
		BNZeroMemory(&element, sizeof(API_Element));
		element.header.guid = e;
		err = ACAPI_Element_Get(&element);
		if (err) throw err;

		LayerUsage(element);
		GeneralAttributeUsage(element);
	}


}

