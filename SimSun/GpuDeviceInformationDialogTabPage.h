#ifndef GPU_DEVICE_INFORMATION_DIALOG_TAB_PAGE_H
#define GPU_DEVICE_INFORMATION_DIALOG_TAB_PAGE_H

#include "ui_GpuDeviceInformationDialogTabPage.h"

struct cudaDeviceProp;

class GpuDeviceInformationDialogTabPage : public QWidget, public Ui::GpuDeviceInformationDialogTabPage
{
	Q_OBJECT

public slots:
	void on_hasChangedMainComputeDevice(int device);

private slots:
	void on_ButtonSetMainComputeDevice_clicked();

signals:
	void setMainComputeDevice(int device);

public:
	GpuDeviceInformationDialogTabPage(const cudaDeviceProp &, int device);
	int deviceNo;
};

#endif