// ContourObj.h: interface for the CContourObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTOUROBJ_H__AC6A2306_787D_4F9D_9CDD_21E0B7EC14E3__INCLUDED_)
#define AFX_CONTOUROBJ_H__AC6A2306_787D_4F9D_9CDD_21E0B7EC14E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*===================等值线绘制对象类===========================*

  一个CContourObj管理一条等值线曲线的绘制，
    
   绘制的内容包括: 轮廓线 ＋ 区域填充

                |-->线型
    轮廓线------|
                |-->线色
   
	            |-->前景色
    区域填充----|
	            |-->背景色
                |
                |-->填充类型

   jeny_man@163.com
*==============================================================*/

#include "GeoStruct.h"
#include "ColorLookUpTable.h"

typedef CArray<CGeoPoint,CGeoPoint&> CCurve; //一条等值线曲线的点集的坐标

class CContourObj  
{
public:
	CContourObj(CCurve*  pCoord, float value);
	~CContourObj();


	
public:

	void Draw(CDC* pDC,
		      const CColorLookUpTable& clrTbl,
		      float scaleRatio, float moveDeltX, float moveDeltY);
	void DrawContours(CDC* pDC, float scaleRatio, float moveDeltX, float moveDeltY);


    void LookUpColor(const CColorLookUpTable& clrTbl); //查找颜色表,设定绘制颜色 
	
private:
	
	//------绘制信息------------------
	CCurve*  m_pCoord; //该等值线的坐标链,指向外部的数据

	float m_Value; //该等值线所对应的值

	COLORREF   m_LineColor;
	//COLORREF  m_FillColor;	

	//--绘制属性管理对象----------
	//
	// CContourLine       m_Line;  //还没有实现
	// CContourRegion     m_Region;   //还没有实现 
private:
	CContourObj& operator=(const CContourObj&); //not implemented
	CContourObj(const CContourObj&);            //not implemented


};

#endif // !defined(AFX_CONTOUROBJ_H__AC6A2306_787D_4F9D_9CDD_21E0B7EC14E3__INCLUDED_)
