#ifndef GPU_DEVICE_INFORMATION_DIALOG_H
#define GPU_DEVICE_INFORMATION_DIALOG_H

#include <QWidget>
#include "ui_GpuDeviceInformationDialog.h"

class GpuDeviceInformationDialog : public QDialog, public Ui::GpuDeviceInformationDialog
{
	Q_OBJECT

public:
	GpuDeviceInformationDialog(QWidget* parent);
	void setupGpuDeviceTabPages();

signals:
	void hasChangedMainComputeDevice(int device);

private slots:
	void on_setMainComputeDevice(int device);

};

#endif