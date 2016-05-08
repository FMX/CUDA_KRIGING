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
*        二维规则网格数据的『等值线追踪』类
*        实现对给定值的等值线追踪
*        追踪算法为：网格方法
*     ----------------------------------------------
*        郑元满   jeny_man  中国地质大学(北京)地信学院
*        jeny_man@163.com  jeny_man@126.com
*        2004/02/14 
*
*     输入：二维规则网格数据，网格数据信息，所要追踪的值，输出的存放位置      
*
*     输出：一个指定值的等值线坐标链表,输出结果放在由外部指定的m_pCurveList中
*      注：该追踪类约定输出的数据结构如下组织:
*
*       //一条等值线曲线的坐标
*      typedef CArray<CGeoPoint,CGeoPoint&> CCurve;
*       //等值线曲线链表(给定某值的等值线不限于一条)
*      typedef CTypedPtrList<CPtrList, CCurve*> CCurveList; 
*      使用该类时，外部需按该约定的数据结构来保存追踪的输出
*
*     用法：在调用ExecuteTracing()前必须调用
*            SetGridDataInfo、SetInput、SetInput
*			这三个函数设置网格数据信息和输入、输出
=========================================================================*/


typedef CArray<CGeoPoint,CGeoPoint&> CCurve; //一条等值线曲线的点集的坐标
typedef CTypedPtrList<CPtrList, CCurve*> CCurveList; //等值线曲线链表(给定某值的等值线不限于一条)

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
    //追踪过程中需要利用如下三个辅助等值点完成追踪： 
	//PreviousPoint点-->前一个等值点
	//CurrentPoint 点-->当前等值点
	//   NextPoint 点-->要追踪的下一个等值点

 /*    网格单元横向、纵向边的序号(编号)

  首先说明的是：该算法实现中，对应于C/C++语言中二维数值用i表示行号，用j表示列号的情况，
                我们这里也用i表示网格数据的行号(向上增加)，j表示网格数据的列号(向右增加)
                而且i,j都是从0开始计数
                标识一个网格交点时，行号在前，列号在右，如：(i,j)
	 
                (i+1,j)       (i+1,j+1)  
	                ____________
                   |            |
				   |            |
	纵边(i,j)----→| (网格单元) |←----纵边(i,j+1)
				   |            |
				   |____________|
                 (i,j)    ∣     (i,j+1)
				          ∣
						  ↓
						  横边(i,j)
       
	  注意：当标识一条边时，说某一条边(i,j)，它即可以表示横边(i,j)，也可以表示纵边(i,j)
	  所以当标识一条边时，需要用三个量来表示(i,j,HorV)-->(行号，列号，横边还是纵边)
	  
*/
	
// 三个辅助等值点所在的横向、纵向网格边的序号,以及坐标
	struct IsoPoint
	{
		int i;   //该等值点所在边的行号  
		int j;   //该等值点所在边的列号
		BOOL bHorV; //在横边还是列边上 1-->横边；0-->纵边

		float x; //坐标
		float y;
		
		IsoPoint(){	memset(this,0,sizeof(IsoPoint));}

	}PreviousPoint,CurrentPoint,NextPoint;
	
	
	//xSide(i,j)---->标识网格横边上是否有当前等值点
	//ySide(i,j)---->标识网格纵边上是否有当前等值点
	//xSide(i,j),ySide(i,j)中存储的值为-2.0(说明该边上无等值点) 或者是 r(r∈[0,1]，说明该边上有等值点)
	float **xSide;
	float **ySide; 

private:
	BOOL IsHavingPoint(float r);  
	void AllocateMemory();
	void FreeMemory();
	void InterpolateTracingValue(); //扫描并计算纵、横边上等值点的情况
	void TracingNextPoint(); //追踪下一个等值点
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

	void HandlingAfterNextPointFounded(int i, int j, BOOL bHorizon); //当下一个等值点找到后做相应的处理
	void CalcAndSaveOnePointCoord(int i, int j, BOOL bHorizon,float &x, float &y);//计算一个等值点的坐标并保存
	
    void TracingOneNonClosedContour();
	void TracingNonClosedContour();
	void TracingOneClosedContour(int i, int j);
	void TracingClosedContour();
	

private:
	//输入数据
	CGridDataInfo m_gridDataInfo;  //网格数据信息
	float**       m_ppGridData;    //网格数据
	float         m_valueTracing;  //当前要追踪的值

	//输出数据的存放位置(需由外部在执行追踪前设定)
	CCurveList*  m_pCurveList;      //存储追踪出来的等值线链的坐标

	//保存输出数据时的帮助变量
	CCurve*     m_currentCurveLine; //指向当前的等值线曲线

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
{//指定输出位置
	ASSERT( pCurveList != NULL );
	m_pCurveList = pCurveList;
}

inline void CContourTracer::SetGridDataInfo(CGridDataInfo& dataInfo)
{
	m_gridDataInfo = dataInfo;
}

inline BOOL CContourTracer::IsHavingPoint(float r)
{
	//边上是否有等值点存在
	
	if( r == -2.0f )
	{
		return FALSE;
	}
	else
	{
		/*r∈[0,1] (闭区间)*/
		if( r>1 || r<0)
			return FALSE;
		else
			return TRUE;
	}
	
}

#endif // !defined(CONTOURTRACER_H_INCLUDED_)
