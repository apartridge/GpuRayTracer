#include "GpuDeviceInformationDialogTabPage.h"
#include <cuda_runtime.h>
#include <QDebug>

GpuDeviceInformationDialogTabPage::GpuDeviceInformationDialogTabPage(const cudaDeviceProp & model, int device_no )
{
	setupUi(this);

	this->deviceNo = device_no;

	GlobalMemory->setText(QString::number(model.totalGlobalMem/(1024*1024)) + QString(" MB"));
	ConstantMemory->setText(QString::number(model.totalConstMem/1024) + QString(" KB"));
	L2CacheMemory->setText(QString::number(model.l2CacheSize/(1024)) + QString(" KB"));
	SharedMemorySM->setText(QString::number(model.sharedMemPerBlock/1024) + QString(" KB"));
	NumSM->setText(QString::number(model.multiProcessorCount));
	ThreadsPerSM->setText(QString::number(model.maxThreadsPerMultiProcessor));
	RegistersSM->setText(QString::number(model.regsPerBlock*4/1024) + QString(" KB (") 
		+ QString::number(model.regsPerBlock) + QString(" registers)"));

	MemClockFq->setText(QString::number(model.memoryClockRate)+QString(" KhZ"));
	PrcClockFrq->setText(QString::number(model.clockRate/1024)+QString(" MhZ"));

	WarpSize->setText(QString::number(model.warpSize));
	ComputeCapability->setText(QString::number(model.major)+ QString(".")+QString::number(model.minor));
	MaxBlockDim->setText(QString::number(model.maxThreadsDim[0]) + QString(" x ") + QString::number(model.maxThreadsDim[1])
						+ QString(" x ") + QString::number(model.maxThreadsDim[2]));
	MaxGridDim->setText(QString::number(model.maxGridSize[0]) + QString(" x ") + QString::number(model.maxGridSize[1])
		+ QString(" x ") + QString::number(model.maxGridSize[2]));
	MaxThreadsPerBlock->setText(QString::number(model.maxThreadsPerBlock));

}

void GpuDeviceInformationDialogTabPage::on_ButtonSetMainComputeDevice_clicked()
{
	emit setMainComputeDevice(this->deviceNo);
}

void GpuDeviceInformationDialogTabPage::on_hasChangedMainComputeDevice( int device )
{
	this->ButtonSetMainComputeDevice->setDisabled(device == this->deviceNo);
}
