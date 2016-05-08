
//=============================================
//
//  定义结构体
//
//============================================

#if !defined(_GEOSTRUCT_H_INCLUDED_)
#define _GEOSTRUCT_H_INCLUDED_

#define GEO_DATATYPE	float		//地理坐标轴的数据类型

struct CGridDataInfo
{
	int rows;
	int cols;
	GEO_DATATYPE xMin;
	GEO_DATATYPE xMax;
	GEO_DATATYPE yMin;
	GEO_DATATYPE yMax;
	float zMin;
	float zMax;

	CGridDataInfo(){memset(this,0,sizeof(CGridDataInfo));}
	
};

struct CGeoPoint
{
	float x;
	float y;

	CGeoPoint(){x=y=0;}
	CGeoPoint(float _x,float _y):x(_x),y(_y){}
	BOOL operator==(const CGeoPoint& rhs){return ( x==rhs.x && y==rhs.y );}
};

struct CGeoRect
{//基于y轴向上的坐标系
	float    left;
    float    top;
    float    right;  //right > left
    float    bottom; // top > bottom
    
	CGeoRect(){memset(this,0,sizeof(CGeoRect));}
	CGeoRect(float l, float b, float r, float t){left=l;right=r;bottom=b;top=t;}
	CGeoRect(const CGeoRect& rhs){left=rhs.left;right=rhs.right;bottom=rhs.bottom;top=rhs.top;}

	float Height()const{return top-bottom;/*top > bottom*/}
	float Width()const{return right-left;}
	float Area()const { return Height()*Width();}
	

	CGeoPoint CenterPoint(){return CGeoPoint((right+left)/2,(top+bottom)/2);}
	void Scale(float ratio)
	{//ration < 1 放大,raion > 1 压缩
		left /= ratio; right /= ratio; top /= ratio; bottom /= ratio;           
	}
	void Translate(float x, float y)
	{//平移
		left += x; right += x; top += y; bottom += y;
	}

	const CGeoPoint& TopLeft()	{ return *((CGeoPoint*)this);}
	const CGeoPoint& BottomRight()	{ return *((CGeoPoint*)this+1);}
//	CGeoPoint TopLeft()       { return CGeoPoint(left,top);     } 
//	CGeoPoint BottomRight()   { return CGeoPoint(right,bottom); }
//	CGeoPoint BottomLeft()    { return CGeoPoint(left,bottom);  }
//	CGeoPoint TopRight()      { return CGeoPoint(right,top);    }

	
};

#endif