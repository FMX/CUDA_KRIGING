// ContourDrawer.cpp: implementation of the CContourDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContourGenerator.h"
#include "Contour.h"
#include "ContourDrawer.h"
#include "ClrFileDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContourDrawer::CContourDrawer(CContour* pContour)
{
	ASSERT(pContour!=NULL);
	m_pContourOwner = pContour;

	m_ContourObjSets.RemoveAll();

	m_ScaleRatio = 1.0f;  
	m_TranslateX = 0.0f;
	m_TranslateY = 0.0f;

	m_ColorTable.SetValueRange(m_pContourOwner->GetDataOwner()->GetDataInfo().zMin,
		                       m_pContourOwner->GetDataOwner()->GetDataInfo().zMax);
}

CContourDrawer::~CContourDrawer()
{
	RemoveAllContourObjs();
}

void CContourDrawer::RemoveAllContourObjs()
{
	int nSize = m_ContourObjSets.GetSize();
	for(int i=0; i<nSize; i++)
	{
		delete m_ContourObjSets[i];
	}

	m_ContourObjSets.RemoveAll();

}

void CContourDrawer::CreateContourObjs()
{//����m_pContourOwner�е�׷�ٳ��ĵ�ֵ�����ߣ�����ContourObj����

	TRACE0("CContourDrawer::CreateContourObjs()\n");

	ASSERT(m_pContourOwner!=NULL);

	RemoveAllContourObjs(); //����֮ǰ�����

	CGridDataInfo& dataInfo = m_pContourOwner->GetDataOwner()->GetDataInfo();
	CGeoRect gridBorder(dataInfo.xMin,dataInfo.yMin,
		                dataInfo.xMax,dataInfo.yMax);
	
	float value;
	CCurveList* pCurveList;
	int numVals = m_pContourOwner->GetNumberOfValues();
	for(int i=0; i<numVals; i++)
	{
		pCurveList = m_pContourOwner->GetContours().GetAt(i);	
		ASSERT( pCurveList!=NULL );

        value = m_pContourOwner->GetValue(i);
		
		POSITION pos = pCurveList->GetHeadPosition();
		CCurve* pCtrLine = NULL;
			
		CContourObj* pCntrObj = NULL;
		while (pos!=NULL)
		{
			pCtrLine = pCurveList->GetNext(pos);
			ASSERT(pCtrLine->GetSize()>=2); //һ����ֵ������������2����

			//����ContourObj����,������������
			pCntrObj = new CContourObj(pCtrLine,value);
			ASSERT(pCntrObj!=NULL);
			m_ContourObjSets.Add(pCntrObj);			
		}
		
	}

}

//---�������ű�����ƽ����---
void CContourDrawer::CalcScaleRatioAndTranslateXY(CDC* pDC,const CRect& drawRect)
{
	int oldMapMode = pDC->GetMapMode();
	pDC->SetMapMode(MM_LOENGLISH); //ʹy������
	
	CRect rt(drawRect);
	pDC->DPtoLP(&rt);
	
	CGridDataInfo& dataInfo = m_pContourOwner->GetDataOwner()->GetDataInfo(); 
	CGeoRect mapRect(dataInfo.xMin,dataInfo.yMin,dataInfo.xMax,dataInfo.yMax);
	
	int rtW = rt.Width();
	int rtH = abs(rt.Height());
	float width  = mapRect.Width();
	float height = mapRect.Height();

	m_ScaleRatio = (width/rtW > height/rtH) ? width/rtW : height/rtH ; //ȡ�������
	
	mapRect.Scale(m_ScaleRatio); //��������任���ƽ����
	m_TranslateX =  rt.CenterPoint().x - mapRect.CenterPoint().x; //x����ƽ����
	m_TranslateY =  rt.CenterPoint().y - mapRect.CenterPoint().y; //y����ƽ����

	pDC->SetMapMode(oldMapMode);

	TRACE3("ratio=%f,Movex=%f,Movey=%f\n",m_ScaleRatio,m_TranslateX,m_TranslateY);
	
}

void CContourDrawer::DoDraw(CDC* pDC, const CRect& drawRect)
{
	//�ѵ�ֵ�����������ʾ�ڻ��ƾ���drawRect��
	ASSERT(m_pContourOwner!=NULL);

	//1.���Contour���³����ˣ�������´���Contour���ƶ���
	if(m_pContourOwner->IsGeneratingAgain())
		CreateContourObjs();

	//2.�ڻ���֮ǰ�����ȼ�������ű�����ƽ����
	CalcScaleRatioAndTranslateXY(pDC,drawRect);
	
	//3.�Ȼ�����߿�
	DrawGridBorder(pDC);
	
	//4.�ٻ���ֵ��
	DrawContours(pDC);

}

//��������߿�
void CContourDrawer::DrawGridBorder(CDC* pDC)
{
	ASSERT( m_pContourOwner!=NULL );

	int oldMapMode = pDC->GetMapMode();
	pDC->SetMapMode(MM_LOENGLISH);
	
	CPen greenPen;
	greenPen.CreatePen(PS_SOLID,1,RGB(0,255,0));
	CPen* oldPen = pDC->SelectObject(&greenPen);

	CGridDataInfo& dataInfo = m_pContourOwner->GetDataOwner()->GetDataInfo(); 
	CGeoRect mapRect(dataInfo.xMin,dataInfo.yMin,dataInfo.xMax,dataInfo.yMax);
		
	mapRect.Scale(m_ScaleRatio);
	mapRect.Translate(m_TranslateX,m_TranslateY);

	pDC->MoveTo(int(mapRect.left), int(mapRect.top));
	pDC->LineTo(int(mapRect.right),int(mapRect.top));
	pDC->LineTo(int(mapRect.right),int(mapRect.bottom));
	pDC->LineTo(int(mapRect.left), int(mapRect.bottom));
	pDC->LineTo(int(mapRect.left), int(mapRect.top));
	
	pDC->SelectObject(oldPen);
	pDC->SetMapMode(oldMapMode);
}

void CContourDrawer::DrawContours(CDC* pDC)
{//����Contour����,�ڻ���֮ǰ���������û���˳�������(����SortDrawOrder����)
	ASSERT( m_pContourOwner!=NULL );
	ASSERT( m_ScaleRatio!=0 );
	
	int oldMapMode = pDC->GetMapMode();
	pDC->SetMapMode(MM_LOENGLISH);

	CContourObj* pContourObj = NULL;
	
	int nSize = m_ContourObjSets.GetSize();

	for(int i=0; i<nSize; i++)
	{
		pContourObj = m_ContourObjSets[i];
		pContourObj->Draw(pDC,m_ColorTable,
			              m_ScaleRatio,m_TranslateX,m_TranslateY);
	}

	pDC->SetMapMode(oldMapMode);

}

void CContourDrawer::ModifyColorTable()
{
	CClrFileDialog Dlg(
		TRUE, // TRUE for FileOpen, FALSE for FileSaveAs
		"clr",
		"*.clr",	//LPCTSTR lpszFileName = NULL,
		OFN_HIDEREADONLY  | OFN_OVERWRITEPROMPT | OFN_ENABLETEMPLATE,
		"Data Files (*.clr) | *.clr ||",
		NULL);
	if(Dlg.DoModal()!=IDOK)
		return;
	
	m_ColorTable.LoadColorTable(Dlg.GetPathName());
	

}