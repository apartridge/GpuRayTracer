#ifndef ResultsDialog_h__
#define ResultsDialog_h__

#include <QWidget>
#include "ui_ResultsDialog.h"

class Window;

class ResultsDialog : public QDialog, public Ui::resultsDialog
{
	Q_OBJECT

public:
	ResultsDialog(Window & window);

public slots:
	void on_pushButton_Close_clicked();
};

#endif // ResultsDialog_h__
