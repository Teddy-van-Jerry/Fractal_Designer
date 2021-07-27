#ifndef FRD_EDITOR_H
#define FRD_EDITOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QLayout>
#include <QTimer>
#include <QSci/qsciscintilla.h>
#include <QSci/qscilexerfrd.h>
#include <QSci/qsciapis.h>

class FRD_Editor : public QWidget
{
    Q_OBJECT

public:
    FRD_Editor(QLayout* layout, int waiting_msec = 0);

    ~FRD_Editor();

    QString text() const;

    void setText(const QString& text);

    void setHighlightWaiting(int msec);

private:
    enum FRD_Indicator {
        FRD_INDIC_CLEAR = -1,
        FRD_INDIC_VARIABLE = 15,
        FRD_INDIC_FUNCTION,
        FRD_INDIC_CLASS,
        FRD_INDIC_ERROR,
        FRD_INDIC_WARNING
    };

    bool isEndString(QChar c) const;

    bool isComment(int pos) const;

    bool isString(int pos) const;

    void setIndicator(int start, int length, FRD_Indicator indicator);

    FRD_Indicator indicatorFromSymbol(QChar symbol) const;

private slots:
    void updateHighlight();

    void waitToUpdateHighlight();

signals:
    QString textChanged();

private:

    QsciScintilla *editor; /**< the FRD editor */

    QLayout* layout;       /**< layout for editor */

    QTimer* timer;

    int waiting_time;
};

#endif // FRD_EDITOR_H
