// ContourValues.cpp: implementation of the CContourValues class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContourGenerator.h"
#include "ContourValues.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContourValues::CContourValues()
{
	m_Values.RemoveAll();
}

CContourValues::~CContourValues()
{
}

CContourValues& CContourValues::operator=(const CContourValues& rhs)
{
	m_Values.RemoveAll();

	POSITION pos = rhs.m_Values.GetHeadPosition();

	while(pos!=NULL)
	{
		m_Values.AddTail(rhs.m_Values.GetNext(pos));
	}

	return *this;
	
}

void CContourValues::AddValue(float newValue)
{//按从小到大插入，插入后保证有序

	if( m_Values.IsEmpty() )
	{
		m_Values.AddTail(newValue);
	}
	else
	{
		float val; 
		POSITION pos = m_Values.GetHeadPosition();
		val = m_Values.GetAt(pos);
		if( newValue < val )
		{
			m_Values.AddHead(newValue);
		//	TraceOutValueList();
			return;
		} 

		pos = m_Values.GetTailPosition();
		val = m_Values.GetAt(pos);
		if( newValue > val)
		{
			m_Values.AddTail(newValue);
			//TraceOutValueList();
			return;
		}

		POSITION nextPos = m_Values.GetHeadPosition();
		pos = nextPos;
		float valNext;
		while( nextPos!=NULL )
		{
			val = m_Values.GetNext(nextPos);
			if( val == newValue )
			{
				//TraceOutValueList();
				return;
			}

			ASSERT(nextPos!=NULL);
			valNext = m_Values.GetAt(nextPos);
			if( newValue == valNext )
			{
				//TraceOutValueList();
				return;
			}

			if( val< newValue && newValue < valNext )
			{
				m_Values.InsertAfter(pos,newValue);
				//TraceOutValueList();
				return;
			}

			pos = nextPos; //watch out here!

			
		}
		
	
	}

	
}

#ifdef _DEBUG
void CContourValues::TraceOutValueList()
{
	
	POSITION pos = m_Values.GetHeadPosition();
	float val;
	while(pos!=NULL)
	{
		val = m_Values.GetNext(pos);
		TRACE1("%f\n",val);
		
	}

}
#endif

void CContourValues::DeleteValue(float oldValue)
{
	POSITION pos = m_Values.GetHeadPosition();
	float val;
	while(pos!=NULL)
	{
		val = m_Values.GetAt(pos);
		if( val == oldValue )
		{
			m_Values.RemoveAt(pos);
		}
		m_Values.GetNext(pos);
		
	}


}

void CContourValues::ReplaceValue(float oldValue, float newValue)
{
	POSITION pos = m_Values.GetHeadPosition();
	float val;
	while(pos!=NULL)
	{
		val = m_Values.GetAt(pos);
		if( val == oldValue )
		{
			m_Values.RemoveAt(pos);
			AddValue(newValue);
		}
		m_Values.GetNext(pos);
		
	}

}

void CContourValues::GenerateValues(int numContours, float rangeStart, float rangeEnd)
{
	float incr;
	
	if (numContours == 1)
    {
		incr = 0;
    }
	else
    {
		incr = (rangeEnd - rangeStart) / (numContours-1);
    }

	float val = rangeStart;
	for (int i=0; i < numContours; i++)
    {
		AddValue(val);
		val+=incr;
    }

}

float CContourValues::GetValue(int i)
{
	ASSERT( i>=0 && i<GetNumberOfValues() );
	return m_Values.GetAt(m_Values.FindIndex(i));
}

void CContourValues::SetValue(int i, float newValue)
{
	int numCoutr = GetNumberOfValues();
	ASSERT( i>=0 && i<numCoutr-1 );

	float val = GetValue(i);
	ReplaceValue(val,newValue);
	
}






















