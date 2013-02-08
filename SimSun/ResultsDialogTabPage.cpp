#include "ResultsDialogTabPage.h"
#include "ResultSet.h"
#include <cmath>

ResultsDialogTabPage::ResultsDialogTabPage(const ResultSet & resultSet)
{
	setupUi(this);
	NumberHits->setText(QString::number(resultSet.numberOfHits));
	NumberRanks->setText(QString::number(resultSet.numberOfRanks));
	PrimaryRays->setText(QString::number(resultSet.primaryRays));
	GeneratedRays->setText(QString::number(resultSet.generatedRays));
	TotalExecutionTime->setText(QString::number(resultSet.totalExecutionTime, 'f') + " s");
	RaysPerSecond->setText(QString::number(floor((resultSet.primaryRays+resultSet.generatedRays)/resultSet.totalExecutionTime)));
	PrimaryRaysPerSecond->setText(QString::number(floor(resultSet.primaryRays/resultSet.totalExecutionTime)));
	GpuTotalTime->setText(QString::number(resultSet.gpuTotalTime, 'f') + " s (" + 
							  QString::number(100*resultSet.gpuTotalTime/resultSet.totalExecutionTime, 'f', 2) + "%)");
	GpuTransferTime->setText(QString::number(resultSet.gpuTransferTime, 'f') + " s (" + 
							 QString::number(100*resultSet.gpuTransferTime/resultSet.gpuTotalTime, 'f', 2) + "%)");
}
