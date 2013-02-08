#ifndef EDITSUNVIEW_H
#define EDITSUNVIEW_H

#include <QDialog>

namespace Ui {
class EditSunView;
}

class EditSunView : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditSunView(QWidget *parent = 0);
    ~EditSunView();
    
private:
    Ui::EditSunView *ui;
};

#endif // EDITSUNVIEW_H
