#include "stdafx.h"
#include "GpuAlgo.h"


GpuAlgo::GpuAlgo()
{
	loadLibrary();
}


GpuAlgo::~GpuAlgo()
{
	unloadLibrary();
}

void GpuAlgo::loadLibrary()
{
	hGpuLibrary = LoadLibrary("gpuLIb.dll");
	if (hGpuLibrary == NULL)
	{
		usable = false;
		AfxMessageBox("Error Occurred while loading CUDA Algorithm!");
		return;
	}


	check = (checkcuda)GetProcAddress(hGpuLibrary, "cudaCapable");
	kringCuda = (kringcuda)GetProcAddress(hGpuLibrary, "kringgpu");
	courtourCuda = (coutourcuda)GetProcAddress(hGpuLibrary, "coutourgpu");
	if (check == NULL || kringCuda == NULL || courtourCuda == NULL)
	{
		usable = false;
		return;
	}

	usable = true;


}

void GpuAlgo::unloadLibrary()
{
	if (hGpuLibrary != NULL)
	{
		FreeLibrary(hGpuLibrary);
	}
}

void GpuAlgo::kring()
{
	return kringCuda();
}

void GpuAlgo::coutourTrace()
{
	return courtourCuda();
}
bool GpuAlgo::isCUDACapable()
{
	return check();
}