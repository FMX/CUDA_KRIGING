#include <stdio.h>

#pragma warning( disable : 4819 )

extern "C"{
	int exec();

	bool cudaCapable();
	
	void kringgpu();

	void coutourgpu();

}