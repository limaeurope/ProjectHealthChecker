#ifndef	_AC_TYPES_
#define	_AC_TYPES_

static const GS::Array<GS::UniString> ac_types{
	"Walls",
	"Columns",
	"Beams",
	"Windows",
	"Doors",
	"Objects",
	"Lamps",
	"Slabs",
	"Roofs",
	"Meshes",

	"Dimensions",
	"RadialDimensions",
	"LevelDimensions",
	"AngleDimensions",

	"Texts",
	"Labels",
	"Zones",

	"Hatches",
	"Lines",
	"PolyLines",
	"Arcs",
	"Circles",
	"Splines",
	"Hotspots",

	"CutPlanes",
	"Cameras",
	"CamSets",

	"Groups",
	"SectElems",

	"Drawings",
	"Pictures",
	"Details",
	"Elevations",
	"InteriorElevations",
	"Worksheets",

	"Hotlinks",

	"CurtainWalls",
	"CurtainWallSegments",
	"CurtainWallFrames",
	"CurtainWallPanels",
	"CurtainWallJunctions",
	"CurtainWallAccessorys",
	"Shells",
	"Skylights",
	"Morphs",
	"ChangeMarkers",
	"Stairs",
	"Risers",
	"Treads",
	"StairStructures",
	"Railings",
	"RailingToprails",
	"RailingHandrails",
	"RailingRails",
	"RailingPosts",
	"RailingInnerPosts",
	"RailingBalusters",
	"RailingPanels",
	"RailingSegments",
	"RailingNodes",
	"RailingBalusterSets",
	"RailingPatterns",
	"RailingToprailEnds",
	"RailingHandrailEnds",
	"RailingRailEnds",
	"RailingToprailConnections",
	"RailingHandrailConnections",
	"RailingRailConnections",
	"RailingEndFinishs",

	"AnalyticalSupports",
	"AnalyticalLinks",

	"BeamSegments",
	"ColumnSegments",
	"Openings",
	"AnalyticalPointLoads",
	"AnalyticalEdgeLoads",
	"AnalyticalSurfaceLoads",
};

static const GS::Array<GS::UniString> ac_mapTypes{
	"Undefined Map",
	"Project Map",
	"PublicView Map",
	"MyView Map",
	"Layout Map",
	"Publisher Sets",
};

static const GS::Array<GS::UniString> ac_navItemTypes{
	"Undefined",
	"Project",
	"Story",
	"Section",
	"DetailDrawing",
	"Perspective",
	"Axonometry",
	"List",
	"Schedule",
	"Toc",
	"Camera",
	"CameraSet",
	"Info",
	"Help",
	"Layout",
	"MasterLayout",
	"Book",
	"MasterFolder",
	"SubSet",
	"TextList",
	"Elevation",
	"InteriorElevation",
	"WorksheetDrawing",
	"DocumentFrom3D",
	"Folder",
	"Drawing",
};

#endif