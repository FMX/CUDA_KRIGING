// GeoData.h: interface for the CGeoData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEODATA_H__5DFAFE72_948C_402A_B470_3721DB3DA1D8__INCLUDED_)
#define AFX_GEODATA_H__5DFAFE72_948C_402A_B470_3721DB3DA1D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoStruct.h"

class CGeoData  
{
public:
	CGeoData();
	virtual ~CGeoData();

public:
	//�����ļ�����
	BOOL ReadData(CString pathName);
	BOOL SaveData(CString pathName);

	//
	CGridDataInfo& GetDataInfo(){return m_DataInfo;}
	float** GetGridData(){return m_ppGridData;}

private:
	CGridDataInfo m_DataInfo;
	float ** m_ppGridData;

private:
	CGeoData(const CGeoData&);
	CGeoData& operator=(const CGeoData&);


};

#endif // !defined(AFX_GEODATA_H__5DFAFE72_948C_402A_B470_3721DB3DA1D8__INCLUDED_)
