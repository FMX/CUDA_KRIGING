// 2DMemAllocator.cpp: implementation of the C2DMemAllocator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContourGenerator.h"
#include "2DMemAllocator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*~~~~~~~二维数组数据动态分配~~~~~~~~~~~~*/
//分配函数:
//AllocMemory2D(T ** &p,int rows,int columns)
//
//释放函数:
//FreeMemory2D(T **&pp)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


C2DMemAllocator::C2DMemAllocator()
{

}

C2DMemAllocator::~C2DMemAllocator()
{

}

void C2DMemAllocator::FreeMemory2D(float **&pp)
{
	//TRACE("in FreeMemory2D\n");
	if(pp)
	{//释放顺序::一维指针-->二维指针
		
		delete []pp[0];
		pp[0]=NULL;
		
		delete []pp;
		pp=NULL;
	}
}


BOOL C2DMemAllocator::AllocMemory2D(float ** &p,int rows,int columns)
{
	
	float **pp;
	pp=NULL;
	pp=new float*[rows];
	if (!pp)
	{
		//TRACE("Alloc memory Open error!!\n");
		return FALSE;
	}
	
	pp[0]=new float [rows*columns];//allocate a big block
	if (!pp[0])
	{
		//TRACE("memeroy Open error!!\n");
		return FALSE;
	}
	for(int i=1;i<rows;i++)
	{
		pp[i]=pp[0]+(long)columns*i;
	}
    
	p = pp;
	
	return TRUE;
}
