#ifndef FRD_EDITOR_H
#define FRD_EDITOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QLayout>
#include <QSci/qsciscintilla.h>
#include <QSci/qscilexerfrd.h>
#include <QSci/qsciapis.h>

class FRD_Editor : QWidget
{
    Q_OBJECT

public:
    FRD_Editor(QLayout* layout);

private:

    QsciScintilla *editor; /**< the FRD editor */

    QLayout* layout;       /**< layout for editor */
};

#endif // FRD_EDITOR_H
