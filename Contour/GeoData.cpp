// GeoData.cpp: implementation of the CGeoData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContourGenerator.h"
#include "GeoData.h"

#include "2DMemAllocator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeoData::CGeoData()
{
	m_ppGridData = NULL;
	memset(&m_DataInfo,0,sizeof(m_DataInfo));
}

CGeoData::~CGeoData()
{
	if( m_ppGridData!=NULL)
	{
		C2DMemAllocator::FreeMemory2D(m_ppGridData);
		m_ppGridData = NULL;
	}

}

//-----------------------------------------
//网格数据存取(读与写)
//-----------------------------------------
BOOL CGeoData::ReadData(CString pathName)
{

    FILE* fp=fopen(pathName,"rt");
	if(!fp)
	{
		AfxMessageBox("Open" + pathName + "error!!");
		return FALSE;
	}
	/////////////////////////////////////////////////

	char head[5];
	//ASCII SURF grid file
	fread( head, sizeof( char ), 4, fp );
	if(!(head[0]=='D' && head[1]=='S' && head[2]=='A' && head[3]=='A'))
	{
		AfxMessageBox("不能打开文件。");
		fclose(fp);
		return FALSE;
	}
	
    //读文件头数据
	float txmin,txmax,tymin,tymax,tcmin,tcmax;
	fscanf(fp,"%d%d%f%f%f%f%f%f",&m_DataInfo.cols,&m_DataInfo.rows,
		                         &txmin,&txmax,
								 &tymin,&tymax,
								 &tcmin,&tcmax);
	m_DataInfo.xMin = txmin;
    m_DataInfo.xMax = txmax;
	m_DataInfo.yMin = tymin;
	m_DataInfo.yMax = tymax;
	m_DataInfo.zMin = tcmin;
	m_DataInfo.zMax = tcmax;


	if( m_ppGridData!=NULL)
	{
		C2DMemAllocator::FreeMemory2D(m_ppGridData);
		m_ppGridData = NULL;
	}

	///////////////////////////////////////
	//读网格数据
    ASSERT( m_ppGridData==NULL );
	if( !C2DMemAllocator::AllocMemory2D(m_ppGridData, m_DataInfo.rows, m_DataInfo.cols) )
	{
		AfxMessageBox("内存分配失败，数据读取没有成功!");
		return FALSE;
	}

	ASSERT( m_ppGridData!=NULL );
	for(int j=0; j<m_DataInfo.rows;j++)
	{
		//making the map looks like in "SURF", so changing the order of data reading
		for(int i=0;i<m_DataInfo.cols;i++)
			fscanf(fp,"%f",&m_ppGridData[j][i]);
	}

	fclose(fp);

	return TRUE;

}


BOOL CGeoData::SaveData(CString pathName)
{
	ASSERT(m_ppGridData!=NULL);

	int i, j;
	float tmp;
	FILE* fpout = NULL;

	
	fpout = fopen(pathName, "wt");
	if(fpout == 0){
		printf("   open file error!!!!!!!!!!!!\n");
		return FALSE;
	}
	float gridZmin= 1.e38f;
	float gridZmax=-1.e38f;
	for(i= 0; i<m_DataInfo.rows; i++)
	{
		for(j=0; j<m_DataInfo.cols; j++)
		{
			tmp=m_ppGridData[i][j];
			if(tmp > gridZmax) gridZmax=tmp;
			if(tmp < gridZmin) gridZmin=tmp;
		}
	}

	m_DataInfo.zMax = gridZmax;
	m_DataInfo.zMin = gridZmin;

	fprintf(fpout,"DSAA\n");
	fprintf(fpout,"%d %d\n",m_DataInfo.cols,m_DataInfo.rows);
	fprintf(fpout,"%f %f\n",m_DataInfo.xMin,m_DataInfo.xMax);
	fprintf(fpout,"%f %f\n",m_DataInfo.yMin,m_DataInfo.yMax);
	fprintf(fpout,"%f %f\n",m_DataInfo.zMin,m_DataInfo.zMax);

	for(i= 0; i<m_DataInfo.rows; i++)
	{
		for(j=0; j<m_DataInfo.cols; j++)
		{
			
			
			if( (i*m_DataInfo.cols+j)%8 == 0 ) 
				fprintf(fpout,"\n");

			fprintf(fpout,"%11.5f ",m_ppGridData[i][j]);
		}
	}

	fclose(fpout);

	return TRUE;
}

