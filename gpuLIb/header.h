#include <stdio.h>

#pragma warning( disable : 4996 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4075 )

extern "C"{
	int exec();

	bool cudaCapable();
	
	void kringgpu();

	void coutourgpu();

}