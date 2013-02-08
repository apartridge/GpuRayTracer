#include "GpuDeviceInformationDialog.h"
#include "GpuDeviceInformationDialogTabPage.h"
#include <QtGui\qmessagebox.h>
#include <QDebug>
#include "cuda_runtime_api.h"

GpuDeviceInformationDialog::GpuDeviceInformationDialog(QWidget* parent) : QDialog(parent)
{
	setupUi(this);
	setupGpuDeviceTabPages();
}

void GpuDeviceInformationDialog::setupGpuDeviceTabPages()
{
	
	int numDevs = 0;
	cudaGetDeviceCount(&numDevs);

	this->setWindowTitle(QString("GPU Device Information (") + QString::number(numDevs) + QString(" devices found)"));

	for(int i = 0; i < numDevs; i++)
	{
		cudaDeviceProp devProp;
		cudaGetDeviceProperties(&devProp, i);

		QWidget* deviceTabPage = new GpuDeviceInformationDialogTabPage(devProp, i);
		
		this->tabWidget->addTab(deviceTabPage, devProp.name);

		connect(deviceTabPage, SIGNAL(setMainComputeDevice(int)), this, SLOT(on_setMainComputeDevice(int)));
		connect(this, SIGNAL(hasChangedMainComputeDevice(int)), deviceTabPage, SLOT(on_hasChangedMainComputeDevice(int)));

	}

	int currentComputeDevice;
	cudaGetDevice(&currentComputeDevice);

	emit hasChangedMainComputeDevice(currentComputeDevice);
}

void GpuDeviceInformationDialog::on_setMainComputeDevice( int device )
{
	int num_devices;
	cudaGetDeviceCount(&num_devices);

	if(device < num_devices)
	{
		cudaDeviceProp devProp;
		cudaGetDeviceProperties(&devProp, device);

		if(devProp.major >= 2)
		{
			cudaSetDevice(device);
			QMessageBox::information(this, tr("Compute Device Changed"), tr("The compute device has been changed \nto \"%1\".").arg(QString(devProp.name)));
			emit hasChangedMainComputeDevice(device);
		}
		else
		{
			QMessageBox::warning(this, tr("Not supported"), tr("The compute device \"%1\" does \nnot support Compute Model 2.0.").arg(QString(devProp.name)));
		}
	}
}

