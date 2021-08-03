#ifndef FRD_EDITOR_H
#define FRD_EDITOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QLayout>
#include <QTimer>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerfrd.h>
#include <Qsci/qsciapis.h>

class FRD_Editor : public QWidget
{
    Q_OBJECT

public:

    enum FRD_Indicator {
        FRD_INDIC_CLEAR = -1,
        FRD_INDIC_VARIABLE = 15,
        FRD_INDIC_FUNCTION,
        FRD_INDIC_CLASS,
        FRD_INDIC_ERROR,
        FRD_INDIC_WARNING,
        FRD_INDIC_SEARCH
    };

    FRD_Editor(QLayout* layout, int waiting_msec = 0);

    ~FRD_Editor();

    QString text() const;

    void setText(const QString& text);

    void setHighlightWaiting(int msec);

    void setSearchIndic(int start, int length);

    void clearSearchIndic(int start, int length);

private:

    bool isEndString(QChar c) const;

    bool isComment(int pos) const;

    bool isString(int pos) const;

    /**
     * \brief set the indicator
     *
     * \details This is used to highlight variable name, function name, class name,
     *          error, warning, and search result.
     * \bug When there are characters taking more than 1 byte,
     *      indicators can be at the wrong position.
     * \param start the starting point of string
     * \param length the length of string
     * \param indicator the type of string
     */
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
