// ContourTracer.h: interface for the CContourTracer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CONTOURTRACER_H_INCLUDED_)
#define CONTOURTRACER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*========================================================================
*        CContourTracer    
*        ��ά�����������ݵġ���ֵ��׷�١���
*        ʵ�ֶԸ���ֵ�ĵ�ֵ��׷��
*        ׷���㷨Ϊ�����񷽷�
*     ----------------------------------------------
*        ֣Ԫ��   jeny_man  �й����ʴ�ѧ(����)����ѧԺ
*        jeny_man@163.com  jeny_man@126.com
*        2004/02/14 
*
*     ���룺��ά�����������ݣ�����������Ϣ����Ҫ׷�ٵ�ֵ������Ĵ��λ��      
*
*     �����һ��ָ��ֵ�ĵ�ֵ����������,�������������ⲿָ����m_pCurveList��
*      ע����׷����Լ����������ݽṹ������֯:
*
*       //һ����ֵ�����ߵ�����
*      typedef CArray<CGeoPoint,CGeoPoint&> CCurve;
*       //��ֵ����������(����ĳֵ�ĵ�ֵ�߲�����һ��)
*      typedef CTypedPtrList<CPtrList, CCurve*> CCurveList; 
*      ʹ�ø���ʱ���ⲿ�谴��Լ�������ݽṹ������׷�ٵ����
*
*     �÷����ڵ���ExecuteTracing()ǰ�������
*            SetGridDataInfo��SetInput��SetInput
*			������������������������Ϣ�����롢���
=========================================================================*/


typedef CArray<CGeoPoint,CGeoPoint&> CCurve; //һ����ֵ�����ߵĵ㼯������
typedef CTypedPtrList<CPtrList, CCurve*> CCurveList; //��ֵ����������(����ĳֵ�ĵ�ֵ�߲�����һ��)

class CContourTracer
{
public:
	CContourTracer();
	~CContourTracer();

	void SetGridDataInfo(CGridDataInfo& dataInfo);
	void SetInput(float** ppGridData);
	void SetOutput(CCurveList* pCurveList);
	BOOL ExecuteTracing(float value);

private:
    //׷�ٹ�������Ҫ������������������ֵ�����׷�٣� 
	//PreviousPoint��-->ǰһ����ֵ��
	//CurrentPoint ��-->��ǰ��ֵ��
	//   NextPoint ��-->Ҫ׷�ٵ���һ����ֵ��

 /*    ����Ԫ��������ߵ����(���)

  ����˵�����ǣ����㷨ʵ���У���Ӧ��C/C++�����ж�ά��ֵ��i��ʾ�кţ���j��ʾ�кŵ������
                ��������Ҳ��i��ʾ�������ݵ��к�(��������)��j��ʾ�������ݵ��к�(��������)
                ����i,j���Ǵ�0��ʼ����
                ��ʶһ�����񽻵�ʱ���к���ǰ���к����ң��磺(i,j)
	 
                (i+1,j)       (i+1,j+1)  
	                ____________
                   |            |
				   |            |
	�ݱ�(i,j)----��| (����Ԫ) |��----�ݱ�(i,j+1)
				   |            |
				   |____________|
                 (i,j)    �O     (i,j+1)
				          �O
						  ��
						  ���(i,j)
       
	  ע�⣺����ʶһ����ʱ��˵ĳһ����(i,j)���������Ա�ʾ���(i,j)��Ҳ���Ա�ʾ�ݱ�(i,j)
	  ���Ե���ʶһ����ʱ����Ҫ������������ʾ(i,j,HorV)-->(�кţ��кţ���߻����ݱ�)
	  
*/
	
// ����������ֵ�����ڵĺ�����������ߵ����,�Լ�����
	struct IsoPoint
	{
		int i;   //�õ�ֵ�����ڱߵ��к�  
		int j;   //�õ�ֵ�����ڱߵ��к�
		BOOL bHorV; //�ں�߻����б��� 1-->��ߣ�0-->�ݱ�

		float x; //����
		float y;
		
		IsoPoint(){	memset(this,0,sizeof(IsoPoint));}

	}PreviousPoint,CurrentPoint,NextPoint;
	
	
	//xSide(i,j)---->��ʶ���������Ƿ��е�ǰ��ֵ��
	//ySide(i,j)---->��ʶ�����ݱ����Ƿ��е�ǰ��ֵ��
	//xSide(i,j),ySide(i,j)�д洢��ֵΪ-2.0(˵���ñ����޵�ֵ��) ������ r(r��[0,1]��˵���ñ����е�ֵ��)
	float **xSide;
	float **ySide; 

private:
	BOOL IsHavingPoint(float r);  
	void AllocateMemory();
	void FreeMemory();
	void InterpolateTracingValue(); //ɨ�貢�����ݡ�����ϵ�ֵ������
	void TracingNextPoint(); //׷����һ����ֵ��
	//{{
	    //-----Version1---------------
		void FromBottom2TopTracing();
		void FromLeft2RightTracing();
		void FromTop2BottomTracing();
		void FromRight2LeftTracing();
		//-----Version2---------------
		void FromBottom2TopTracingV2();
		void FromLeft2RightTracingV2();
		void FromTop2BottomTracingV2();
		void FromRight2LeftTracingV2();
    //}}

	void HandlingAfterNextPointFounded(int i, int j, BOOL bHorizon); //����һ����ֵ���ҵ�������Ӧ�Ĵ���
	void CalcAndSaveOnePointCoord(int i, int j, BOOL bHorizon,float &x, float &y);//����һ����ֵ������겢����
	
    void TracingOneNonClosedContour();
	void TracingNonClosedContour();
	void TracingOneClosedContour(int i, int j);
	void TracingClosedContour();
	

private:
	//��������
	CGridDataInfo m_gridDataInfo;  //����������Ϣ
	float**       m_ppGridData;    //��������
	float         m_valueTracing;  //��ǰҪ׷�ٵ�ֵ

	//������ݵĴ��λ��(�����ⲿ��ִ��׷��ǰ�趨)
	CCurveList*  m_pCurveList;      //�洢׷�ٳ����ĵ�ֵ����������

	//�����������ʱ�İ�������
	CCurve*     m_currentCurveLine; //ָ��ǰ�ĵ�ֵ������

private:
	CContourTracer(const CContourTracer& rhs); //not implimented
	CContourTracer& operator=(const CContourTracer& rhs);//not implimented
		
};

inline void CContourTracer::SetInput(float** ppGridData)
{
	ASSERT( ppGridData != NULL );
	m_ppGridData = ppGridData;
}

inline void CContourTracer::SetOutput(CCurveList* pCurveList)
{//ָ�����λ��
	ASSERT( pCurveList != NULL );
	m_pCurveList = pCurveList;
}

inline void CContourTracer::SetGridDataInfo(CGridDataInfo& dataInfo)
{
	m_gridDataInfo = dataInfo;
}

inline BOOL CContourTracer::IsHavingPoint(float r)
{
	//�����Ƿ��е�ֵ�����
	
	if( r == -2.0f )
	{
		return FALSE;
	}
	else
	{
		/*r��[0,1] (������)*/
		if( r>1 || r<0)
			return FALSE;
		else
			return TRUE;
	}
	
}

#endif // !defined(CONTOURTRACER_H_INCLUDED_)
