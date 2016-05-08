// ContourTracer.cpp: implementation of the CContourTracer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContourGenerator.h"
#include "ContourTracer.h"
#include "2DMemAllocator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CContourTracer::CContourTracer()
{
	m_ppGridData = NULL;

	m_pCurveList = NULL;
	m_currentCurveLine = NULL;

	xSide = NULL;
	ySide = NULL;

}

CContourTracer::~CContourTracer()
{
	FreeMemory();	
}



BOOL CContourTracer::ExecuteTracing(float value)
{
	ASSERT( m_gridDataInfo.cols != 0 ); //在调用该函数之前必须调用SetGridDataInfo()函数设置网格数据信息

	if( value < m_gridDataInfo.zMin && value > m_gridDataInfo.zMax )
	{
		return FALSE;
	}


	m_valueTracing = value;


	//1.为xSide和ySide分配内存空间
	AllocateMemory();

	//2.扫描网格纵横边，内插等值点
	//该函数之中，在计算等值点时，发现追踪值与网格点上的数据相同时，
	//在计算前，会都数据做修正(加上一小偏移值)
	InterpolateTracingValue(); 

	//3.先追踪开曲线
	TracingNonClosedContour();

	//4.再追踪闭曲线
	TracingClosedContour();

	//5.释放空间
	//FreeMemory();

	return TRUE;
	



	
}

void CContourTracer::AllocateMemory()
{// 分配xSide，ySide空间
	int cols = m_gridDataInfo.cols;
	int rows = m_gridDataInfo.rows;
	
	if( xSide == NULL )
	{
		//网格中存在rows*(cols-1)条横边，所有需要为xSide分配rows*(cols-1)空间就行

		C2DMemAllocator::AllocMemory2D(xSide,rows,cols-1/*not cols*/);
	}
	
	if( ySide == NULL )
	{
		//网格中存在(rows-1)*cols条纵边，所有需要为ySide分配(rows-1)*cols空间就行

		C2DMemAllocator::AllocMemory2D(ySide,rows-1/*not rows*/,cols);
	}
	
}

void CContourTracer::FreeMemory()
{
	
	if( xSide != NULL )
	{
		C2DMemAllocator::FreeMemory2D(xSide);
		xSide = NULL;
	}
	
	if( ySide != NULL )
	{
		C2DMemAllocator::FreeMemory2D(ySide);
		ySide = NULL;
	}
	
}


//-----消除网格交点奇异Z值------
//在自动网格边上追踪等值点时，有时会遇到网格交点上的Z值与追踪的等值线值相等的情况;
//如果不对这种情况进行处理，网格追踪算法就会出现错误，一般的处理如下：
//将网格交点上的z值加或减一个修正值，来消除奇异点的影响

//void CContourTracer::AmendingData(BOOL  bForTracing  /*是否为追纵算法而修正数据*/, 
//							   float fValueTracing/*所要追踪的等值线值*/)
//{
//
//	ASSERT( m_ppGridData!=NULL );
//
//	int i,j;
//
//	float shift = 0.001f;  //修正值
//
//	if(bForTracing)
//	{
//		//若把与所要追踪的值相同的加修正值
//		for(i=0; i<m_gridDataInfo.rows; i++)
//		{
//			for(j=0; j<m_gridDataInfo.cols; j++)
//			{
//				if( m_ppGridData[i][j] == fValueTracing )  
//					m_ppGridData[i][j] += shift;    //should changing back!!
//			}
//		}
//	}
//	else
//	{
//		float shiftVal = fValueTracing + shift;
//
//		for(i=0; i<m_gridDataInfo.rows; i++)
//		{
//			for(j=0; j<m_gridDataInfo.cols; j++)
//			{
//				if( m_ppGridData[i][j] == shiftVal )  
//					m_ppGridData[i][j] -= shift;    //restore original data
//			}
//		}
//	}
//}

//扫描网格的纵、横边，并线性插值计算等值点的情况
//将各边上的等值点情况存储于xSide和ySide数组中，
// xSide存储所有横边上的等值线情况, ySide存储所有纵边上的等值点情况
//在插值计算时，对『与追踪值相等的数据』要进行修正处理后才计算，但在做修正处理时不要改变原来的数据
void CContourTracer::InterpolateTracingValue()
{
/*      网格点标识如下:
        
            (i+1,j)·--------·(i+1,j+1)
                    |        |
                    |        |
                    |        |
	                |        |
	         (i,j) ·--------·(i,j+1)

              i:表示行号(向上增加)
			  j:表示列号(向右增加)
			  标识一个网格交点时，行号在前，列号在右，如：(i,j)
*/
/*        xSide,ySide中存储r值，(w为追踪值)
          对于网格横边，r = (w - pData[i][j]) / (pData[i][j+1]-pData[i][j]);
		  对于网格纵边，r = (w - pData[i][j]) / (pData[i+1][j]-pData[i][j]);

		  由于浮点运算的误差，xSide[i][j],ySide[i][j]有可能等于1.0或0.0 
		  考虑如下情况：
		  1。当追踪值与网格点上的值很接近(但不相等)时，由于运算误差，就会等于1.0
		     比如追踪0值时，遇到如下边:
			       20 ·--------·-0.00000016   此边上有0值，但计算 (0-20)/(-0.00000016-20) == 1.0 

       
		  2。当网格边上两端点上的值相差很悬殊时。
		     比如追踪2值，遇到如下边：
		     1.70141E+038 ·--------·1   此边上有2值，计算(2-1.70141E+038) / (1-1.70141E+038) == 1.0 

        网格边上有等值点时，理论上比例值不会等于0或1；
		 但由于计算误差，我们在算法中判断时，认为0或1也是有等值点的

        所以xSide,ySide中存储的值是[0,1]的闭区间，不是(0,1)的开区间
*/
	ASSERT( m_ppGridData!=NULL );
	ASSERT( xSide != NULL );
	ASSERT( ySide != NULL );
	
	int i,j;
	
	int   rows = m_gridDataInfo.rows;
	int   cols = m_gridDataInfo.cols;
	float  w   = m_valueTracing; 
	float** pData = m_ppGridData;
	
	float H1,H2; //分别记录一条边的两个点上的数据值
	float flag; 
	float shift = 0.001f;  //修正值


	/* 扫描并计算横边上的等值点,有rows*(cols-1)条横边需要扫描*/
	for(i=0; i<rows ; i++)
	{
		for(j=0;j<cols-1;j++)
		{				
			/*考查横边(i,j)上的左交点(i,j)上的值pData[i][j]
			                 和右交点(i,j+1)的值pData[i][j+1]*/
//			if( pData[i][j] == pData[i][j+1] )
//			{
//				// -2表示此边无等值点
//				xSide[i][j] = -2.0f;
//			}
//			else
//			{
//				xSide[i][j] = (w - pData[i][j]) / ( pData[i][j+1] - pData[i][j] );
//				
//				if( ( xSide[i][j] <= 0 )  || ( xSide[i][j] >= 1 ) )
//					xSide[i][j] = -2.0f;
//			}

			H1 = pData[i][j]; 			H2 = pData[i][j+1];		

			if( H1 == H2 )
			{
				xSide[i][j] = -2.0f;
			}
			else
			{
				flag = (w-H1) * (w-H2);
				if( flag > 0 )
				{
					xSide[i][j] = -2.0f;
				}
				else if( flag < 0 )
				{
					xSide[i][j] = (w-H1) / (H2-H1) ; 
					ASSERT(xSide[i][j]>=0 && xSide[i][j]<=1.0f);
				}
				else if( flag == 0)
				{//其中有一值与追踪值w相等，则修正之(加上一小值偏移量)
					if( H1 == w )
					{
						H1 += shift;
					}
					else
					{
						H2 += shift;
					}
					
					xSide[i][j] = (w-H1) / (H2-H1) ; 
				//	ASSERT(xSide[i][j]>=0 && xSide[i][j]<=1.0f);

				}
				else
				{
					ASSERT(FALSE);
				}

			}
			
			


		}
	}

	/* 扫描并计算纵边上等值点,有(rows-1)*cols条纵边需要扫描*/
	for(i=0; i<rows-1;i++)
	{
		for(j=0; j<cols; j++)
		{			
			/*考查纵边(i,j)上的下交点(i,j)上的值pData[i][j]
			                 和上交点(i+1,j)的值pData[i+1][j]*/

//			if( pData[i][j] == pData[i+1][j] )
//			{
//				/* -2表示此边无等值点，或已追踪过，以后不再考虑*/
//				ySide[i][j] = -2.0f;
//			}
//			else
//			{
//				ySide[i][j] = (w - pData[i][j]) / ( pData[i+1][j] - pData[i][j] );
//				
//				if( ( ySide[i][j] <= 0 )  || ( ySide[i][j] >= 1 ) )
//					ySide[i][j] = -2.0f;
//			}
                                             
			H1 = pData[i][j];            
			H2 = pData[i+1][j];		     
                                         
			if( H1 == H2 )               
			{
				ySide[i][j] = -2.0f;
			}
			else
			{
				flag = (w-H1) * (w-H2);
				if( flag > 0 )
				{
					ySide[i][j] = -2.0f;
				}
				else if( flag < 0 )
				{/*
				 网格边上有等值点时，存储的值∈[0,1] (闭区间)
                 */
					ySide[i][j] = (w-H1) / (H2-H1) ; 
					ASSERT(ySide[i][j]>=0 && ySide[i][j]<=1.0f);
				}
				else if( flag == 0 )
				{//如果其中有一值与追踪值w相等，则修正之(加上一小值偏移量)
					if( H1 == w )
					{
						H1 += shift;
					}
					else
					{
						H2 += shift;
					}
					
					ySide[i][j] = (w-H1) / (H2-H1) ; 
				//	ASSERT(ySide[i][j]>=0 && ySide[i][j]<=1.0f);
				}
				else
				{
					ASSERT(FALSE);
				}


			}

		}
	}
}

inline void CContourTracer::CalcAndSaveOnePointCoord(int i, int j, BOOL bHorizon,float &x, float &y)
{
	/*static*/ float deltX = (m_gridDataInfo.xMax - m_gridDataInfo.xMin) / ( m_gridDataInfo.cols - 1 );
	/*static*/ float deltY = (m_gridDataInfo.yMax - m_gridDataInfo.yMin) / ( m_gridDataInfo.rows - 1 );
	// should not be static, because diffirent input data has diffirent xyMin Max
	
	
	if( bHorizon )
	{//在横边上
		x = m_gridDataInfo.xMin + ( j + xSide[i][j] ) * deltX;
		y = m_gridDataInfo.yMin + i * deltY;		
	}
	else
	{//在纵边上
		x = m_gridDataInfo.xMin + j * deltX;
		y = m_gridDataInfo.yMin + ( i + ySide[i][j] ) * deltY;
	}

	//Saving Coord
	CGeoPoint point(x,y);
	m_currentCurveLine->Add(point);

}

//当下一个等值点找到后做相应的处理
void CContourTracer::HandlingAfterNextPointFounded(int i, int j, BOOL bHorizon)
{//参数说明：i,j分别是等值点所在边的编号，bHorizon指明所在边是横边还是纵边

	//当下一个等值点找到后做相应的处理,如下：
	//1.记录该等值点的i,j
	//2.计算并保存该等值点的坐标
	//3.标志该等值点所在边的已经搜索过

	//验证i∈[0,rows-1], j∈[0,cols-1]
	ASSERT( i>=0 && i<=m_gridDataInfo.rows-1 && j>=0 && j<=m_gridDataInfo.cols-1 );

	//1.
	NextPoint.i = i;
	NextPoint.j = j;
    NextPoint.bHorV = bHorizon;

	//2.
	CalcAndSaveOnePointCoord(i,j,bHorizon,NextPoint.x,NextPoint.y);

	//3.
	if( NextPoint.bHorV )
	{
		xSide[i][j] = -2.0f; //已经追踪过
	}
	else
	{	
		ySide[i][j] = -2.0f; //已经追踪过
	}

}

void CContourTracer::TracingNextPoint()
{
/*  
  1.先确定出等值线的前进方向(自下向上、由左向右、自上向下、由右向左，其中之一)
  2.再追踪下一个等值点

  前进方向可以如下判定：
    if( 当前点.行号 > 前一点.行号 )
    {
	    下---->上
	}
	else if( 当前点.列号 > 前一点.列号 )
    {
		左---->右
	}
    else if( 当前点在横边上 )
	{
		上---->下
	}
	else
	{
	     右---->左
	}
       
*/

#define  _CALL_TRACINGFUN_VERSION2

	if(CurrentPoint.i > PreviousPoint.i )
	{
		//TRACE0("下--->上\n");
		#ifdef _CALL_TRACINGFUN_VERSION2
			FromBottom2TopTracingV2();
		#else
			FromBottom2TopTracing();
		#endif
	}
	else if(CurrentPoint.j > PreviousPoint.j )
	{
		//TRACE0("左--->右\n");
		#ifdef _CALL_TRACINGFUN_VERSION2
			FromLeft2RightTracingV2();
		#else
			FromLeft2RightTracing();
		#endif
		
	}
	else if( CurrentPoint.bHorV == TRUE /*curPt_Col_X < CurrentPoint.x*/ ) 
	{//在横边上。CurrentPoint.bHorizon == TRUE 和 curPt_Col_X < CurrentPoint.x ，这两个条件等价
		
		
		ASSERT( CurrentPoint.i <= PreviousPoint.i &&
			    CurrentPoint.j <= PreviousPoint.j );

		//TRACE0("上--->下\n");
		#ifdef _CALL_TRACINGFUN_VERSION2
			FromTop2BottomTracingV2();
		#else
			FromTop2BottomTracing();
		#endif
	
	}
	else
	{
		ASSERT(CurrentPoint.bHorV==FALSE);//在纵边上

		ASSERT( CurrentPoint.i <= PreviousPoint.i &&
				CurrentPoint.j <= PreviousPoint.j );
		
		//TRACE0("右--->左\n");
		#ifdef _CALL_TRACINGFUN_VERSION2
			FromRight2LeftTracingV2();
		#else
			FromRight2LeftTracing();
		#endif
	
	}
	
}

//自下向上追踪下一等值点
void CContourTracer::FromBottom2TopTracing()
{
/*   
	                   横边(i+1,j)
                      xSide[i+1][j]

                           ↑
				(i+1,j)    ∣    (i+1,j+1)
					|-----------|
					|           |
	  纵边(i,j)---→|           |←----纵边(i,j+1)
      ySide[i][j]	|           |     ySide[i][j+1]
	                |           |
	                |-----·----|    
				(i,j)      ↖     (i,j+1)
				             ＼
							   等前等值点(P2)
        
		  等值线自下向上前进时，网格单元的情况如上：
		  当前等值点(用P2表示)在网格单元的底边上，那么下一等值点(用P3表示)所在的位置有三种情况：
		     1.在纵边(i,j)上
			 2.在纵边(i,j+1)上
			 3.在横边(i+1,j)上
		 但实际追踪时只能选择其中之一

         程序判断的依据即是：ySide[i][j] 、 ySide[i][j+1] 、xSide[i+1][j] 

       假设纵边(i,j)上存在等值点P31, 纵边(i,j+1)上存在等值点P33, 横边(i+1,j)上存在等值点P32
	   选择的次序如下：
	   1.当P31，P33都存在时，选择靠近网格底边者为P3(比较ySide[i][j]、ySide[i][j+1])
	   2.若P31,P33靠近底边的距离相同，则选择与P2点距离近者为P3
	   3.当P31,P33中只有一个存在时，则存在点即为P3
	   4.当无P31,P33存在时，对边必定存在P32作为P3

*/
	ASSERT( CurrentPoint.i > PreviousPoint.i );   //当前点的行号 > 前一点的行号
	ASSERT( CurrentPoint.bHorV == TRUE );
	

	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;

	  

	if( IsHavingPoint(ySide[i][j]) && IsHavingPoint(ySide[i][j+1]) )
	{
		
		if( ySide[i][j] < ySide[i][j+1] )
		{
			HandlingAfterNextPointFounded(i,j,FALSE);//在纵边(i,j)上
		}
		else if( ySide[i][j] == ySide[i][j+1] )
		{
			//注意:这里需要重新计算xSide[i][j]，不能直接用xSide[i][j],因为在上一次已经被置为-2.0
			float xSideIJ = (m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i][j+1] - m_ppGridData[i][j] );
			if( xSideIJ <= 0.5f )
			//if( xSide[i][j] <= 0.5f )
			{
				TRACE3("xSide[%d][%d]=%f,左-->右----遇到两边r值相等\n",i,j,xSide[i][j]);
				HandlingAfterNextPointFounded(i,j,FALSE);//在纵边(i,j)上
			}
			else
			{
				HandlingAfterNextPointFounded(i,j+1,FALSE);//在纵边(i,j+1)上
			}

		}
		else
		{
			HandlingAfterNextPointFounded(i,j+1,FALSE);//在纵边(i,j+1)上
		}

	}
	else
	{
		if( IsHavingPoint(ySide[i][j]) )
		{
			HandlingAfterNextPointFounded(i,j,FALSE);//在纵边(i,j)上
		}
		else if( IsHavingPoint(ySide[i][j+1]) )
		{
			HandlingAfterNextPointFounded(i,j+1,FALSE);//在纵边(i,j+1)上
		}
		else if( IsHavingPoint(xSide[i+1][j]) )
		{//两纵边上都没有
			HandlingAfterNextPointFounded(i+1,j,TRUE);//在横边(i+1,j)上
		}
		else
		{//三边上都没有,数据插值出现错误
			ASSERT(FALSE);
		}
	}

}

//自下向上追踪函数(第二版本)
void CContourTracer::FromBottom2TopTracingV2()
{
/*	                   横边(i+1,j)
                      xSide[i+1][j]

                           ↑
				(i+1,j)    ∣    (i+1,j+1)
					|-----------|
					|           |
	  纵边(i,j)---→|           |←----纵边(i,j+1)
      ySide[i][j]	|           |     ySide[i][j+1]
	                |           |
	                |-----·----|    
				(i,j)      ↖     (i,j+1)
				             ＼
							   等前等值点
*/
	ASSERT( CurrentPoint.i > PreviousPoint.i );   //当前点的行号 > 前一点的行号
	ASSERT( CurrentPoint.bHorV == TRUE );
	

	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;

	if( ySide[i][j] < ySide[i][j+1] )
	{
		if( ySide[i][j] > 0 )
			HandlingAfterNextPointFounded(i,j,FALSE);
		else
			HandlingAfterNextPointFounded(i,j+1,FALSE);
	}
	else if( ySide[i][j] == ySide[i][j+1] )
	{
		if( ySide[i][j] < 0 /*namely, ySide[i][j] == ySide[i][j+1] == -2.0*/)
		{
			HandlingAfterNextPointFounded(i+1,j,TRUE);
		}
		else
		{
			float xSideIJ = (m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i][j+1] - m_ppGridData[i][j] );
			if( xSideIJ <=0.5f )
			{
				HandlingAfterNextPointFounded(i,j,FALSE);
			}
			else
			{
				HandlingAfterNextPointFounded(i,j+1,FALSE);
			}
		}
	}
	else if( ySide[i][j] > ySide[i][j+1] )
	{
		if( ySide[i][j+1] > 0 )
			HandlingAfterNextPointFounded(i,j+1,FALSE);
		else
			HandlingAfterNextPointFounded(i,j,FALSE);		
	}
}

//由左向右追踪
void CContourTracer::FromLeft2RightTracing()
{
/*   
	                   横边(i+1,j)
                      xSide[i+1][j]
                           ∣ 
                           ∣
				(i+1,j)    ↓    (i+1,j+1)
					|-----------|
					|           |
 等前等值点(P2)--→·           |←----纵边(i,j+1)
     	            |           |     ySide[i][j+1]
	                |           |
	                |-----------|    
				(i,j)     ↑   (i,j+1)
				          ∣   
						  横边(i,j)
						 xSide[i][j]	   
*/
	ASSERT( CurrentPoint.j > PreviousPoint.j );
	ASSERT( CurrentPoint.bHorV == FALSE );

	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;	
	if( IsHavingPoint(xSide[i][j]) && IsHavingPoint(xSide[i+1][j]) )
	{
		
		if( xSide[i][j] < xSide[i+1][j] )
		{
			HandlingAfterNextPointFounded(i,j,TRUE);//在横边(i,j)上
		}
		else if( xSide[i][j] == xSide[i+1][j] )
		{
			//注意:这里需要重新计算ySide[i][j]，因为在上一次已经被置为-2.0
			float ySideIJ = ( m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i+1][j] - m_ppGridData[i][j] );
			if( ySideIJ <= 0.5f )
			//if( ySide[i][j] <= 0.5f )
			{
				TRACE3("ySide[%d][%d]=%f,左-->右----遇到两边r值相等\n",i,j,ySide[i][j]);
				HandlingAfterNextPointFounded(i,j,TRUE);//在横边(i,j)上
			}
			else
			{
				HandlingAfterNextPointFounded(i+1,j,TRUE);//在横边(i+1,j)上
			}
			
		}
		else
		{
			HandlingAfterNextPointFounded(i+1,j,TRUE);//在横边(i+1,j)上
		}
		
	}
	else
	{
		if( IsHavingPoint(xSide[i][j]) )
		{
			HandlingAfterNextPointFounded(i,j,TRUE);//在横边(i,j)上
		}
		else if( IsHavingPoint(xSide[i+1][j]) )
		{
			HandlingAfterNextPointFounded(i+1,j,TRUE);//在横边(i+1,j)上
		}
		else if( IsHavingPoint(ySide[i][j+1]) )
		{//两横边上都没有
			HandlingAfterNextPointFounded(i,j+1,FALSE);//在纵边(i,j+1)上
		}
		else
		{//三边上都没有
			ASSERT(FALSE);
		}
	}
	
}

//由左向右追踪函数(第二版本)
void CContourTracer::FromLeft2RightTracingV2()
{
	ASSERT( CurrentPoint.j > PreviousPoint.j );
	ASSERT( CurrentPoint.bHorV == FALSE );
	
	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;	

	if( xSide[i][j] < xSide[i+1][j] )
	{
		if( xSide[i][j] > 0 )
			HandlingAfterNextPointFounded(i,j,TRUE);
		else
			HandlingAfterNextPointFounded(i+1,j,TRUE);
	}
	else if( xSide[i][j] == xSide[i+1][j] )
	{
		if( xSide[i][j] < 0 /*namely, xSide[i][j] == xSide[i+1][j] == -2.0*/)
		{
			HandlingAfterNextPointFounded(i,j+1,FALSE);
		}
		else
		{		
			float ySideIJ = ( m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i+1][j] - m_ppGridData[i][j] );
			if( ySideIJ <= 0.5f )
				HandlingAfterNextPointFounded(i,j,TRUE);
			else
				HandlingAfterNextPointFounded(i+1,j,TRUE);
		}
	}
	else if( xSide[i][j] > xSide[i+1][j] )
	{
		if( xSide[i+1][j] > 0 )
			HandlingAfterNextPointFounded(i+1,j,TRUE);
		else
			HandlingAfterNextPointFounded(i,j,TRUE);

	}


}

//由上向下
void CContourTracer::FromTop2BottomTracing()
{
/*   
	                   等前等值点(P2)
                          ∣
                          ∣
				(i,j)     ↓    (i,j+1)
					|-----·----|
					|           |
	纵边(i-1,j)---→|           |←----纵边(i-1,j+1)
    ySide[i-1][j]	|           |     ySide[i-1][j+1]
	                |           |
	                |-----------|    
			 (i-1,j)      ↑     (i-1,j+1)
				          ∣   
						横边(i-1,j)
					   xSide[i-1][j]
            
*/

	ASSERT( CurrentPoint.bHorV == TRUE );	

	int i = CurrentPoint.i;
	int j = CurrentPoint.j;

	if( IsHavingPoint(ySide[i-1][j]) && IsHavingPoint(ySide[i-1][j+1]) )
	{
		
		if( ySide[i-1][j] < ySide[i-1][j+1] )
		{
			HandlingAfterNextPointFounded(i-1,j,FALSE);//在纵边(i-1,j)上
		}
		else if( ySide[i-1][j] == ySide[i-1][j+1] )
		{
			//注意:这里需要重新计算xSide[i][j]，因为在上一次已经被置为-2.0
			float xSideIJ = ( m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i][j+1] - m_ppGridData[i][j] );
			if( /*xSide[i][j]*/ xSideIJ <= 0.5f )
			{
				TRACE0("xSide[i][j] <= 0.5f\n");
				HandlingAfterNextPointFounded(i-1,j,FALSE);//在纵边(i-,j)上
			}
			else
			{
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);//在纵边(i-1,j+1)上
			}
			
		}
		else
		{
			HandlingAfterNextPointFounded(i-1,j+1,FALSE);//在纵边(i-1,j+1)上
		}
		
	}
	else
	{
		if( IsHavingPoint(ySide[i-1][j]) )
		{
			HandlingAfterNextPointFounded(i-1,j,FALSE);//在纵边(i-1,j)上
		}
		else if( IsHavingPoint(ySide[i-1][j+1]) )
		{
			HandlingAfterNextPointFounded(i-1,j+1,FALSE);//在纵边(i-1,j+1)上
		}
		else if( IsHavingPoint(xSide[i-1][j]) )
		{//两纵边上都没有
			HandlingAfterNextPointFounded(i-1,j,TRUE);//在横边(i-1,j)上
		}
		else
		{//三边上都没有
			ASSERT(FALSE);
		}
	}

}


//从上到下追踪子函数的第二版本
void CContourTracer::FromTop2BottomTracingV2()
{
	
	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;


	
	//比较：ySide[i-1][j]      ySide[i-1][j+1]
	//                 
	//               xSide[i-1][j] 
	
	if( ySide[i-1][j] < ySide[i-1][j+1] )
	{
		if( ySide[i-1][j] > 0 )
			HandlingAfterNextPointFounded(i-1,j,FALSE);
		else
			HandlingAfterNextPointFounded(i-1,j+1,FALSE);
	}
	else if( ySide[i-1][j] == ySide[i-1][j+1] )
	{
		if( ySide[i-1][j] < 0 /*即ySide[i-1][j] == ySide[i-1][j+1] ==-2*/) 
		{
			HandlingAfterNextPointFounded(i-1,j,TRUE);//下一点在对面的横边上
		}
		else
		{
			
			//注意:这里需要重新计算xSide[i][j]，因为在上一次已经被置为-2.0
			float xSideIJ = ( m_valueTracing - m_ppGridData[i][j] ) / ( m_ppGridData[i][j+1] - m_ppGridData[i][j] );
			if(  xSideIJ <= 0.5f )
			{
				HandlingAfterNextPointFounded(i-1,j,FALSE);
			}
			else
			{
				HandlingAfterNextPointFounded(i-1,j+1,FALSE);
			}
		}
	}
	else if( ySide[i-1][j] > ySide[i-1][j+1] )
	{
		if( ySide[i-1][j+1] > 0 )
			HandlingAfterNextPointFounded(i-1,j+1,FALSE);
		else
			HandlingAfterNextPointFounded(i-1,j,FALSE);
	}
	
	
}
		
//由右向左
void CContourTracer::FromRight2LeftTracing()
{
/*   
	                   横边(i+1,j-1)
                      xSide[i+1][j-1]
                           ∣ 
                           ∣
			  (i+1,j-1)    ↓    (i+1,j)
					|-----------|
					|           |
    纵边(i,j-1)--→ |           ·←----等前等值点(P2)
   ySide[i][j-1]    |           |     
	                |           |
	                |-----------|    
			  (i,j-1)     ↑   (i,j)
				          ∣   
						  横边(i,j-1)
						 xSide[i][j-1]	   
*/

	ASSERT( CurrentPoint.bHorV == FALSE );
	

	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;	

	if( IsHavingPoint(xSide[i][j-1]) && IsHavingPoint(xSide[i+1][j-1]) )
	{
		
		if( xSide[i][j-1] < xSide[i+1][j-1] )
		{
			HandlingAfterNextPointFounded(i,j-1,TRUE);//在横边(i,j-1)上
		}
		else if( xSide[i][j] == xSide[i+1][j-1] )
		{
			//注意:这里需要重新计算ySide[i][j]，因为在上一次已经被置为-2.0
			float ySideIJ = ( m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i+1][j] - m_ppGridData[i][j] );
			if( /*ySide[i][j]*/ ySideIJ <= 0.5f )
			{
				HandlingAfterNextPointFounded(i,j-1,TRUE);//在横边(i,j-1)上
			}
			else
			{
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);//在横边(i+1,j-1)上
			}
			
		}
		else
		{
			HandlingAfterNextPointFounded(i+1,j-1,TRUE);//在横边(i+1,j-1)上
		}
		
	}
	else
	{
		if( IsHavingPoint(xSide[i][j-1]) )
		{
			HandlingAfterNextPointFounded(i,j-1,TRUE);//在横边(i,j-1)上
		}
		else if( IsHavingPoint(xSide[i+1][j-1]) )
		{
			HandlingAfterNextPointFounded(i+1,j-1,TRUE);//在横边(i+1,j-1)上
		}
		else if( IsHavingPoint(ySide[i][j-1]) )
		{//两横边上都没有
			HandlingAfterNextPointFounded(i,j-1,FALSE);//在纵边(i,j-1)上
		}
		else
		{//三边上都没有
			ASSERT(FALSE);
		}
	}
}

//由右向左(第二版本)
void CContourTracer::FromRight2LeftTracingV2()
{/*   
	                   横边(i+1,j-1)
                      xSide[i+1][j-1]
                           ∣
			  (i+1,j-1)    ↓    (i+1,j)
					|-----------|
					|           |
    纵边(i,j-1)--→ |           ·←----等前等值点(P2)
   ySide[i][j-1]    |           |     
	                |           |
	                |-----------|    
			  (i,j-1)     ↑   (i,j)
				          ∣   
						  横边(i,j-1)
						 xSide[i][j-1]	   
*/
	ASSERT( CurrentPoint.bHorV == FALSE );
	int i = CurrentPoint.i; 
	int j = CurrentPoint.j;	

	if( xSide[i][j-1] < xSide[i+1][j-1] )
	{
		if( xSide[i][j-1] > 0 )
			HandlingAfterNextPointFounded(i,j-1,TRUE);
		else
			HandlingAfterNextPointFounded(i+1,j-1,TRUE);
	}
	else if( xSide[i][j-1] == xSide[i+1][j-1] )
	{
		if( xSide[i][j-1] < 0 /*namely, xSide[i][j-1] == xSide[i+1][j-1] == -2.0f*/ )
		{
			HandlingAfterNextPointFounded(i,j-1,FALSE);
		}
		else
		{
			float ySideIJ = ( m_valueTracing - m_ppGridData[i][j]) / ( m_ppGridData[i+1][j] - m_ppGridData[i][j] );
			if( ySideIJ <= 0.5f )
				HandlingAfterNextPointFounded(i,j-1,TRUE);
			else
				HandlingAfterNextPointFounded(i+1,j-1,TRUE);
		}
	}
	else if( xSide[i][j-1] > xSide[i+1][j-1] )
	{
		if( xSide[i+1][j-1] > 0 )
			HandlingAfterNextPointFounded(i+1,j-1,TRUE);
		else
			HandlingAfterNextPointFounded(i,j-1,TRUE);
		
	}
	

	
}

//追踪一条开等值线
void CContourTracer::TracingOneNonClosedContour()
{
	

	CCurve* pCtrLine = new CCurve;
	m_pCurveList->AddTail(pCtrLine);
	
	m_currentCurveLine = pCtrLine;
	
	//记录下线头所在边的i，j，横纵边标识
	int startPt_i = CurrentPoint.i; 
	int startPt_j = CurrentPoint.j;
	BOOL startPt_bHoriz = CurrentPoint.bHorV;
	
	//验证线头在边界上
	ASSERT( startPt_i == 0                     ||  /*线头在底边界上*/
		startPt_i == m_gridDataInfo.rows-1 ||  /*线头在上边界上*/
		startPt_j == 0                     ||  /*线头在左边界上*/
		startPt_j == m_gridDataInfo.cols-1     /*线头在右边界上*/ 
		);

	CalcAndSaveOnePointCoord(startPt_i,startPt_j,startPt_bHoriz,CurrentPoint.x,CurrentPoint.y); //计算出线头的坐标并保存

	
	if( startPt_bHoriz )
	{
		xSide[startPt_i][startPt_j] = -2.0f;
	}
	else
	{
		ySide[startPt_i][startPt_j] = -2.0f;
	}
	
	TracingNextPoint(); //追踪出NextPoint
	
	PreviousPoint = CurrentPoint;
	CurrentPoint = NextPoint;
	
	int cols = m_gridDataInfo.cols;
	int rows = m_gridDataInfo.rows;
	
	
	//遇到网格边界就结束追踪
	
	//------2004/03/09修改------------------
	//为了不让浮点数计算的误差引起追踪结束条件的判断失误，
	//我们不能用『CurrentPoint.y <= yMin』来判断是否遇到底边框,
	//  也不能用『CurrentPoint.x <= xMin』来判断是否遇到左边框
	BOOL bIsFinish = (CurrentPoint.i == 0 && CurrentPoint.bHorV == TRUE )  ||  /*遇到底边界,(注:不能仅仅用CurrentPoint.i == 0判断)*/
		(CurrentPoint.i == rows-1)                            ||  /*遇到上边界*/
		(CurrentPoint.j == 0 && CurrentPoint.bHorV == FALSE ) ||  /*遇到左边界,(注:不能仅仅用CurrentPoint.j == 0判断)*/
		(CurrentPoint.j == cols-1) ;                              /*遇到右边界*/
	
	
	
	
	while( !bIsFinish )
	{
		TracingNextPoint();
		
		PreviousPoint = CurrentPoint;
		CurrentPoint = NextPoint;
		
		bIsFinish = (CurrentPoint.i == 0 && CurrentPoint.bHorV == TRUE )  ||
			(CurrentPoint.i == rows-1)                            || 
			(CurrentPoint.j == 0 && CurrentPoint.bHorV == FALSE ) ||
			(CurrentPoint.j == cols-1) ;
		
	}
}

//追踪开曲线
void CContourTracer::TracingNonClosedContour()
{//该函数的关键是找出开曲线的线头，并设置好追踪前进的方向的条件
	
	int i,j;
	int cols = m_gridDataInfo.cols;
	int rows = m_gridDataInfo.rows;	

	//1.搜索底边框	
	for(j=0; j<cols-1; j++)
	{
		if( IsHavingPoint(xSide[0][j]) )
		{
			/*按自下向上的前进方向虚设前一点的i,j*/
			PreviousPoint.i = -1; // 假设前一点在-1行,这样下一点到当前点的前进方向就是自下向上的
			PreviousPoint.j = j;
			PreviousPoint.bHorV = TRUE;

			CurrentPoint.i = 0; //底边的行号为0，所以设置线头的i为0
			CurrentPoint.j = j;
			CurrentPoint.bHorV = TRUE; //底边是横边

			TracingOneNonClosedContour();	
		}
	}

	//2.搜索左边框(左边框在第0列)
	for(i=0; i<rows-1; i++)
	{
		if( IsHavingPoint(ySide[i][0]) )
		{
			/*按由左向右的前进方向虚设前一点的i,j*/
			PreviousPoint.i = i; 
			PreviousPoint.j = -1; //假设前一点在-1列,使其满足左-->右的前进方向
			PreviousPoint.bHorV = FALSE;
			
			CurrentPoint.i = i; 
			CurrentPoint.j = 0; //左边框在第0列，所以设置线头的j为0
			CurrentPoint.bHorV = FALSE; 
			
			TracingOneNonClosedContour();	
		}
	}

	//3.搜索上边框

	float deltX = ( m_gridDataInfo.xMax-m_gridDataInfo.xMin ) / ( m_gridDataInfo.cols-1 );
    float curPt_Col_X = m_gridDataInfo.xMin + CurrentPoint.j * deltX; //当前等值点所在边(i,j)的所标识的j列的X坐标
	
	for(j=0; j<cols-1; j++)
	{
		if( IsHavingPoint(xSide[rows-1][j]) )
		{
			/*虚设出由上向下追踪的条件
			  由上向下追踪的条件如下： 	 
			  1. Not( CurrentPoint.i > PreviousPoint.i )
			  2. Not( CurrentPoint.j > PreviousPoint.j )
			  3. CurrentPoint.x > m_gridDataInfo.xMin + CurrentPoint.j * deltX; 即:要求在横边上
			*/
			PreviousPoint.i = rows-1; 
			PreviousPoint.j = j;
			PreviousPoint.bHorV = TRUE;
			
			CurrentPoint.i = rows-1; //上边框的行号为rows-1
			CurrentPoint.j = j;
			CurrentPoint.bHorV = TRUE; //使其符合第三个条件


			//第三个条件条件不用虚设，只要边上存在就会满足第三个条件，因为上边框是横边
//			if( CurrentPoint.x < curPt_Col_X )
//			{
//				CurrentPoint.x = curPt_Col_X + 1; //为了保证让其满足第3个条件
//			}
			
			
		 	
			TracingOneNonClosedContour();	
		}
	}

	//4.搜索右边框(在第cols-1列，注意列号是从0开始计数的)

	
	for(i=0; i<rows-1; i++)
	{
		if( IsHavingPoint(ySide[i][cols-1]) )
		{
			/*虚设出由右向左追踪的条件
			
			  由右向左追踪的条件如下： 	 
			  1. Not( CurrentPoint.i > PreviousPoint.i )
			  2. Not( CurrentPoint.j > PreviousPoint.j )
			  3. Not( CurrentPoint.x > m_gridDataInfo.xMin + CurrentPoint.j * deltX ); 要求在纵边上			
			*/
			PreviousPoint.i = i; 
			PreviousPoint.j = cols-1; 
			PreviousPoint.bHorV = FALSE;
			
			CurrentPoint.i = i; 
			CurrentPoint.j = cols-1; //右边框在第cols-1列
			CurrentPoint.bHorV = FALSE;  //使其符合第三个条件

//			//虚设出第三个条件
//			CurrentPoint.x = curPt_Col_X - 1;//减去1，使之比curPt_Col_X小
			
			TracingOneNonClosedContour();	
		}
	}
	
}

void CContourTracer::TracingClosedContour()
{
	
	
	int cols = m_gridDataInfo.cols;
	int rows = m_gridDataInfo.rows;

	//搜索所有的除了边框外的纵边,从左到右搜索每一列上的纵边，对于一列，从下到上搜索
	for(int j=1; j<cols-1; j++) //j从1开始
		for(int i=0; i<rows-1; i++) //i从0开始
		{
			if( IsHavingPoint(ySide[i][j])) 
			{
				TracingOneClosedContour(i,j);				
			}
			
		}
}

//从纵边开始追踪一条封闭等值线(注：是从纵边开始搜索)
void CContourTracer::TracingOneClosedContour(int startPt_i, int startPt_j)
{//参数i,j是该闭曲线的第一个等值点的i,j

	CCurve* pCtrLine = new CCurve;
	
	m_pCurveList->AddTail(pCtrLine);
	m_currentCurveLine = pCtrLine;
	
	//虚设前一等值点的i,j，让其满足从左向右追踪的条件
	PreviousPoint.i = startPt_i;
	PreviousPoint.j = 0;
	PreviousPoint.bHorV = FALSE;
	
	CurrentPoint.i = startPt_i;
	CurrentPoint.j = startPt_j;
	CurrentPoint.bHorV = FALSE;   //是FALSE，因为是在纵边上
	
	
	
	
	//	float startPt_x;
	//	float startPt_y;
	CalcAndSaveOnePointCoord(startPt_i,startPt_j,FALSE/*线头在纵边上*/,CurrentPoint.x,CurrentPoint.y);
	
	TracingNextPoint();
	
	PreviousPoint = CurrentPoint;
	CurrentPoint = NextPoint;
	
	BOOL bColsed = FALSE;
	
	while(!bColsed) 
	{
		TracingNextPoint();
		
		PreviousPoint = CurrentPoint;
		CurrentPoint = NextPoint;
		
		//------2004/03/09修改------------------
		//用等值点所在边的标识i,j来判断曲线是否封闭，不用x、y坐标来比较，因为浮点数计算会出现误差
		//	bColsed = (CurrentPoint.x == startPt_x) && (CurrentPoint.y == startPt_y);
		bColsed = ( CurrentPoint.i == startPt_i ) && 
			( CurrentPoint.j == startPt_j ) && 
			( CurrentPoint.bHorV == FALSE );
		
		
	}
	
	ySide[startPt_i][startPt_j] = -2.0f; //已经追踪过
}

