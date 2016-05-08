// ContourDrawer.h: interface for the CContourDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTOURDRAWER_H__0BD72FC9_023F_4CDA_85EF_CB9EAED3883A__INCLUDED_)
#define AFX_CONTOURDRAWER_H__0BD72FC9_023F_4CDA_85EF_CB9EAED3883A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*===================��ֵ�߻�����===========================*/
//
//  �为���ֵ�ߵĻ����Լ��������ԵĹ���
//   jeny_man@163.com
/*==============================================================*/

#include "ContourObj.h"

typedef CTypedPtrArray<CPtrArray,CContourObj*> CContourObjArray;

class CContour;
class CContourDrawer  
{
public:
	CContourDrawer(CContour* pContour);
	virtual ~CContourDrawer();

	void CreateContourObjs();
	void DoDraw(CDC* pDC, const CRect& drawRect);
	void ModifyColorTable();
	//---------�����������--------------------
	//  SetColorTable();.....
	//  SetDrawStyle();......

private:
	//-----�������---------------
	void CalcScaleRatioAndTranslateXY(CDC* pDC,const CRect& drawRect);
	void DrawGridBorder(CDC* pDC);
	void DrawContours(CDC* pDC);
	

	//-----���ƶ���-------------
	void RemoveAllContourObjs(); //������е�ContourObj����

private:
	CContour* m_pContourOwner; //��Ҫ���Ƶĵ�ֵ��
	CContourObjArray m_ContourObjSets; //���ƶ��󼯺�
	CColorLookUpTable m_ColorTable;
	
	//------��������--------------
	//   
	//   CDrawStyle......

	//------ʵ�����ź�ƽ��--------------
	float m_ScaleRatio; //���ű���
	float m_TranslateX; //X����ƽ����
	float m_TranslateY; //Y����ƽ����

};

#endif // !defined(AFX_CONTOURDRAWER_H__0BD72FC9_023F_4CDA_85EF_CB9EAED3883A__INCLUDED_)
