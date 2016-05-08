// ColorLookUpTable.cpp: implementation of the CColorLookUpTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContourGenerator.h"
#include "ColorLookUpTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorLookUpTable::CColorLookUpTable()
{
	m_ValueMin = m_ValueMax = 0;

	//缺省颜色表为RainBow颜色表
	m_ColorTable.Add( CColorNode(0.f,RGB(153,102,255)) );
	m_ColorTable.Add( CColorNode(0.2f,0x0000FF) );
	m_ColorTable.Add( CColorNode(0.4f,0x00FF00) );
	m_ColorTable.Add( CColorNode(0.6f,0xFFFF00) );
	m_ColorTable.Add( CColorNode(0.8f,RGB(255,102,0)) );
	m_ColorTable.Add( CColorNode(1.f,0xFF0000) );
}

CColorLookUpTable::~CColorLookUpTable()
{

}

//根据pos的位置找出在ColorTable中对应的颜色,
//pos为0～1的值
COLORREF CColorLookUpTable::GetColor(float pos) const
{
	ASSERT( pos>=0.f && pos<=1.0f );
	
	if(pos<=m_ColorTable.GetAt(0).pos)
		return m_ColorTable.GetAt(0).color;
	
	
	int count=m_ColorTable.GetSize();
	float ps = 0;
	
	for(int i=0; i<count; i++)
	{
		ps = m_ColorTable[i].pos;

		if( pos == ps)
		{
			return m_ColorTable[i].color;
		}
		else if( pos > ps )
		{
			continue;
		}
		else if( pos < ps )
		{ 
			float ratio = (                 pos - m_ColorTable[i-1].pos ) / 
				          ( m_ColorTable[i].pos - m_ColorTable[i-1].pos );

			COLORREF color = m_ColorTable[i-1].color;
			BYTE s_r = GetRValue(color);
			BYTE s_g = GetGValue(color);
			BYTE s_b = GetBValue(color);
			
			color = m_ColorTable[i].color;
			BYTE e_r = GetRValue(color);
			BYTE e_g = GetGValue(color);
			BYTE e_b = GetBValue(color);
			
			return RGB(s_r+(e_r-s_r)*ratio,
				       s_g+(e_g-s_g)*ratio,
				       s_b+(e_b-s_b)*ratio);
		}
	}

	return m_ColorTable.GetAt(count-1).color;
	

}

void CColorLookUpTable::SetColor(float pos, COLORREF newClr)
{
	ASSERT( pos>=0.f && pos<=1.0f );

	InsertColorIntoTable(pos,newClr);


}

 //在颜色表中查询v值的颜色
COLORREF CColorLookUpTable::LookUpColor(float v) const
{
	ASSERT( v>=m_ValueMin && v<=m_ValueMax );

	return GetColor( (v-m_ValueMin)/(m_ValueMax-m_ValueMin) );
}

//向颜色表中加入颜色节点,根据pos从小到大顺序插入,同时返回插入后所在的位置
int CColorLookUpTable::InsertColorIntoTable(float pos, COLORREF newClr)
{
	ASSERT( pos>=0.f && pos<=1.0f );

	int count = m_ColorTable.GetSize();

	float p = 0.f;
	for(int i=0; i<count; i++)
	{
		p = m_ColorTable.GetAt(i).pos;
		if( pos > p)
		{			
			continue;
		}
		else if( pos == p )
		{
            m_ColorTable[i].color = newClr;  //修改
			return i;
		}
		else if( pos < p  )
		{
			m_ColorTable.InsertAt(i,CColorNode(pos,newClr));
			return i;
		}
	}
	
	return m_ColorTable.Add(CColorNode(pos,newClr));
}

//删除颜色表中的颜色节点
void CColorLookUpTable::RemoveColorFromTable(float pos)
{
	int nSize = m_ColorTable.GetSize();

	for(int i=0; i<nSize; i++)
	{
		if( m_ColorTable.GetAt(i).pos == pos )
		{
			m_ColorTable.RemoveAt(i);
		}
	}

}

void CColorLookUpTable::GradientRectangle(CDC* pDC, const CRect& drawRect,
										  COLORREF clrStart,COLORREF clrEnd)
{//从clrStart开始到clrEnd渐变填充矩形drawRect
	

	int width  = drawRect.Width();
	int height = drawRect.Height();

	CPoint pt0 = drawRect.TopLeft(); //左上角
	CPoint pt1 = drawRect.BottomRight(); //右下角

	BYTE r,g,b;

	//按像素填充颜色
	COLORREF fillClr;

    for(int i=pt0.x; i<pt1.x; i++)
	{
		//线性插值
		 r = i*( GetRValue(clrEnd) - GetRValue(clrStart) ) / width + ( pt1.x*GetRValue(clrStart) - pt0.x*GetRValue(clrEnd) )/width;
		 g = i*( GetGValue(clrEnd) - GetGValue(clrStart) ) / width + ( pt1.x*GetGValue(clrStart) - pt0.x*GetGValue(clrEnd) )/width;
		 b = i*( GetBValue(clrEnd) - GetBValue(clrStart) ) / width + ( pt1.x*GetBValue(clrStart) - pt0.x*GetBValue(clrEnd) )/width;
		 
		 fillClr = RGB(r,g,b);
		
		for(int j=pt0.y; j<pt1.y; j++)
		{
			//MemDC.SetPixelV(i,j,RGB(r,g,b));
			pDC->SetPixelV(i,j,fillClr);
		}
	}

 

}

//绘制颜色表的颜色谱
void CColorLookUpTable::DrawColorSpectrum(CDC* pDC, const CRect& drawRect)
{
	int nSize = m_ColorTable.GetSize();

	CRect rect(drawRect);
    
	for(int i=1; i<nSize; i++)//i从1开始
	{
		rect.right = drawRect.left + int(drawRect.Width()*m_ColorTable.GetAt(i).pos);

		GradientRectangle(pDC,rect,m_ColorTable.GetAt(i-1).color,m_ColorTable.GetAt(i).color);
		
		rect.left = rect.right;
	}
	
}

BOOL CColorLookUpTable::LoadColorTable(const CString& filePath )
{
	FILE* fp=fopen(filePath,"rt");

	if(!fp)
	{
		AfxMessageBox("Open error!!");
		return FALSE;
	}

	char head[13]="";
	int headlen=strlen("ColorMap 1 1");
	fread( head, sizeof( char ), headlen, fp );
	if(strcmp(head,"ColorMap 1 1")!=0)
	{
		AfxMessageBox("文件格式有误，不能打开文件.");
		fclose(fp);
		return FALSE;
	}

	//-------开始读取颜色表------------------
	m_ColorTable.RemoveAll();

	UINT r,g,b;
	float pos;
	while(!feof(fp))
	{
		fscanf(fp,"%f%d%d%d",&pos, &r, &g, &b);
		m_ColorTable.Add( CColorNode(pos/100.f,RGB(r,g,b)) );
		if(pos>=100.f)
			break;
	}

	fclose(fp);

	return TRUE;

}

BOOL CColorLookUpTable::SaveColorTable(const CString& filePath )
{
     //Don't call
	//not impemented.......

	return FALSE; 

}


