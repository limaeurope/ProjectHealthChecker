#if !defined _API_TYPE_DICT_H
#define _API_TYPE_DICT_H


class APITypeDict {
public:
	GS::HashTable<GS::UniString, API_LibraryTypeID> table;

	APITypeDict();

	API_LibraryTypeID GetLibPartType(const API_LibPart& i_libPart) const;
};

#endif //_API_TYPE_DICT_H