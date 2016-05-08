// 2DMemAllocator.h: interface for the C2DMemAllocator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(C2DMEMALLOCATOR_H_INCLUDED_)
#define C2DMEMALLOCATOR_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/***********************************************************************
//        C2DMemAllocator    
//        
//        二维数组动态分配（采用先整体分配，再指针定位思想）
//
//        分配函数:
//        AllocMemory2D(T ** &p,int rows,int columns)
//        释放函数:
//        FreeMemory2D(T **&pp)        
//
//----------------------------------------------
//        郑元满   jeny_man
//        2004/01/14 
***********************************************************************/

class C2DMemAllocator  
{
public:
	C2DMemAllocator();
	~C2DMemAllocator();

    
	/*~~~~~~~~~分配函数~~~~~~~~~~~~~~~~~*/
	//三个参数：
	//   p: [out],该函数分配成功的指针设置给p
	//rows: [in], 行数
	//cols: [in], 列数
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	static BOOL AllocMemory2D(float ** &p,int rows,int cols);


	/*~~~~~~~~~释放函数~~~~~~~~~~~~~~~~~*/
	//释放由AllocMemory2D()分配成功的内存空间
	//一个参数：
	//   pp: [in],由AllocMemory2D()分配的内存指针
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	static void FreeMemory2D(float **&pp);

};

#endif // !defined(C2DMEMALLOCATOR_H_INCLUDED_)
