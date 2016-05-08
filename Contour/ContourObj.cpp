// ContourObj.cpp: implementation of the CContourObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContourGenerator.h"
#include "ContourObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContourObj::CContourObj(CCurve*  pCoord, float value)
{
	ASSERT( pCoord != NULL );
	m_pCoord = pCoord;
	m_Value = value;

	//m_FillColor = RGB(255,0,0);
	m_LineColor = RGB(0,0,0);

}

CContourObj::~CContourObj()
{
	//��Ҫ������delete m_pCoord;
	TRACE0("CContourObj::~CContourObj()\n");
}


void CContourObj::LookUpColor(const CColorLookUpTable& clrTbl)
{//����ɫ���в���m_Value��Ӧ����ɫֵ
	m_LineColor = clrTbl.LookUpColor(m_Value);
}


void CContourObj::Draw(CDC* pDC, const CColorLookUpTable& clrTbl,
					   float scaleRatio, float moveDeltX, float moveDeltY)
{//�ѵ�ֵ�����������ʾ��rt��

    //1.�ڻ���֮ǰ�Ȳ��Һ���ɫ
	LookUpColor(clrTbl);

	//2.���Ƶ�ֵ��
	DrawContours(pDC,scaleRatio, moveDeltX, moveDeltY);
		
}



void CContourObj::DrawContours(CDC* pDC, float scaleRatio, float moveDeltX, float moveDeltY)
{
	ASSERT( m_pCoord!=NULL );
	
	int nSize = m_pCoord->GetSize();
	ASSERT(nSize>=2); 

	CPen pen(PS_SOLID,1,m_LineColor);
	CPen *pOldPen=pDC->SelectObject(&pen);
	
	

	pDC->MoveTo(int(m_pCoord->GetAt(0).x/scaleRatio + moveDeltX),
		        int(m_pCoord->GetAt(0).y/scaleRatio + moveDeltY));

	for(int i=1;i<nSize;i++)
	{//����ӳ��任(�ȱ����任��ƽ��)

		pDC->LineTo(int(m_pCoord->GetAt(i).x/scaleRatio+moveDeltX),
			        int(m_pCoord->GetAt(i).y/scaleRatio+moveDeltY));
	}
	
	

	pDC->SelectObject(pOldPen);


}










































