#pragma once

//CUDA Ëã·¨°ü×°

typedef bool (*checkcuda)();
typedef void (*kringcuda)();
typedef void (*coutourcuda)();

class GpuAlgo
{
private:
	HINSTANCE hGpuLibrary;
	checkcuda check;
	kringcuda kringCuda;
	coutourcuda courtourCuda;
	bool usable;
	void loadLibrary();
	void unloadLibrary();
public:
	GpuAlgo();
	~GpuAlgo();
	void kring();
	void coutourTrace();
	bool isCUDACapable();

};

