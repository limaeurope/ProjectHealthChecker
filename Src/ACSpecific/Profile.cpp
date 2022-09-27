#include	"Profile.hpp"
#include	"../Table/Table.hpp"
#include	"VectorImageIterator.hpp"
#include	"../SettingsSingleton.hpp"

AbstractData* GetArcNumber(const API_Attribute& i_apiAttrib, AbstractData* i_attrs)
{
	API_AttributeDefExt defs;
	GSErrCode err;
	PolygonReportObject*result = new PolygonReportObject;
	result->nArcs = 0;

	err = ACAPI_Attribute_GetDefExt(i_apiAttrib.header.typeID, i_apiAttrib.header.index, &defs);

	result->name = i_apiAttrib.header.name;

	VectorImage vi = defs.profile_vectorImageItems->GetVectorImage();

	ConstVectorImageIterator cvii{ vi };

	while (!cvii.IsEOI())
	{
		const Sy_HatchType* sht = cvii;

		if (sht)
		{
			HatchObject hatchObject = vi.GetHatchObject(*sht);

			result->nArcs += hatchObject.GetArcs().GetSize();
		}

		cvii++;
	}

	return result;
}

void ProcessProfiles()
{
	ResultSheet rs{ "Profile data" };
	SettingsSingleton::GetInstance().ResultTable.sheetDict.Add(rs.sName, rs);
	rs.header = GS::Array<GS::UniString>{ "Profile data", "Number of Profiles" };

	//AddItem("Layer data", "Number of Materials", CountAttributes(API_ProfileID), io_cntlDlgData);

	GS::Array<AbstractData*> profileS = ListAttributes(API_ProfileID, GetArcNumber);

	for (AbstractData* prof : profileS)
	{
		PolygonReportObject* _prof = (PolygonReportObject*)prof;
		//AddItem("Profile data", _prof->name, (UInt16)_prof->nArcs, io_cntlDlgData);
		rs.AddItem("Profile data", _prof->name, (UInt16)_prof->nArcs);

		delete prof;
	}
}
