#include "ResultsDialog.h"
#include "ResultSet.h"
#include "Window.h"
#include <QVector>
#include "ResultsDialogTabPage.h"

ResultsDialog::ResultsDialog(Window & window) : QDialog((Window* const)&window)
{
	setupUi(this);
	const QVector<ResultSet> & results = window.getResultSets();

	for(int i = results.size()-1; i >= 0; i--)
	{
		QWidget* deviceTabPage = new ResultsDialogTabPage(results[i]);
		this->resultsTabWidget->addTab(deviceTabPage, QString("ResultSet ") + QString::number(i));
	}

}

void ResultsDialog::on_pushButton_Close_clicked()
{
	this->close();
}

