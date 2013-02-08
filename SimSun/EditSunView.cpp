#include "EditSunView.h"
#include "ui_editsunview.h"

EditSunView::EditSunView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditSunView)
{
    ui->setupUi(this);
}

EditSunView::~EditSunView()
{
    delete ui;
}
