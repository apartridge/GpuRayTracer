#ifndef ResultsDialogTabPage_h__
#define ResultsDialogTabPage_h__

#include "ui_ResultsDialogTabPage.h"

class ResultSet;

class ResultsDialogTabPage : public QWidget, public Ui::resultsDialogTabPage
{
	Q_OBJECT

public:
	ResultsDialogTabPage(const ResultSet & resultSet);
};

#endif