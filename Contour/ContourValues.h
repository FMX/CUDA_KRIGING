// ContourValues.h: interface for the CContourValues class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__CONTOURVALUES_H_INCLUDE)
#define __CONTOURVALUES_H_INCLUDE

#if _MSC_VER > 1000
#pragma once
#endif

/*=============等值线值的辅助管理类=============================*/
//
//该类辅助CContour类，用于编辑和管理所要追踪的等值线值
//
/*==============================================================*/

typedef CList<float,float> CValueList;

class CContourValues  
{
public:
	CContourValues();
	~CContourValues();

	CContourValues& operator=(const CContourValues& rhs);

public:
	void AddValue(float newValue);
	void DeleteValue(float oldValue);
	void RemoveAllValues();
	void ReplaceValue(float oldValue, float newValue);
	void GenerateValues(int numContours, float rangeStart, float rangeEnd);
	int  GetNumberOfValues();
	float GetValue(int i);
	void SetValue(int i, float newValue);

	 CValueList& GetValueList(){ return m_Values; }

#ifdef _DEBUG
	 void TraceOutValueList();
#endif

private:
	 CValueList m_Values;   //要追踪的值

private:
	CContourValues(const CContourValues&);  // Not implemented.


};

inline int CContourValues::GetNumberOfValues() 
{
	return m_Values.GetCount();
}

inline void CContourValues::RemoveAllValues()
{
	m_Values.RemoveAll();
}
#endif
