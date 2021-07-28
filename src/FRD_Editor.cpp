#include "FRD_Editor.h"

FRD_Editor::FRD_Editor(QLayout* layout_, int waiting)
    : layout(layout_), waiting_time(waiting)
{
    editor = new QsciScintilla(this);
    timer = new QTimer(this);

    QsciLexerFRD* textLexer = new QsciLexerFRD; // create a lexer highlighter
    editor->setLexer(textLexer);
    // set font as Consolas
    textLexer->setFont(QFont("Consolas"));

    // set line width
    // A positive value increases the letter spacing by the corresponding pixels;
    // a negative value decreases the spacing.
    editor->setLineWidth(QFont::AbsoluteSpacing);

    // set line number and the margin style
    editor->setMarginType(0, QsciScintilla::NumberMargin);
    editor->setMarginLineNumbers(0, true);
    editor->setMarginWidth(0, 40);

    // auto completion
    QsciAPIs* apis = new QsciAPIs(textLexer);
    if (!apis->load(":/Keywords/Global_Variables.txt") ||
        !apis->load(":/Keywords/Keywords.txt") ||
        !apis->load(":/Keywords/Maths_Functions.txt") ||
        !apis->load(":/Keywords/Preset_Classes.txt") ||
        !apis->load(":/Keywords/Preset_Func.txt"))
    {
        qDebug() << "Failed to read files";
    }
    else
    {
        apis->prepare();
    }

    // set the source of auto completion
    editor->setAutoCompletionSource(QsciScintilla::AcsAll);

    // set auto completion to be case sensitive
    editor->setAutoCompletionCaseSensitivity(true);

    // show auto completion tip when 2 characters are typed
    editor->setAutoCompletionThreshold(2);

    // set UTF-8
    editor->SendScintilla(QsciScintilla::SCI_SETCODEPAGE, QsciScintilla::SC_CP_UTF8);

    // set auto indent
    editor->setAutoIndent(true);

    // selection color
    editor->setSelectionBackgroundColor(Qt::black); // selected text background colour
    editor->setSelectionForegroundColor(Qt::white); // selected text forefront colour

    // set selected line colour
    editor->setMarginBackgroundColor(0, Qt::lightGray);
    editor->setMarginsForegroundColor(Qt::black);

    // folding style and colour
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    editor->setFoldMarginColors(Qt::gray,Qt::lightGray);

    // brace matching
    editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);

    // set tab width
    editor->setTabWidth(4);

    // About Caret
    editor->setCaretWidth(2);                            // cursor width
    editor->setCaretForegroundColor(QColor("darkCyan")); // cursor colour
    editor->setCaretLineVisible(true);                   // highlight current line
    editor->setCaretLineBackgroundColor(0xF5FFFA);       // background colour of the current line (mintcream)

    // Indicators (Used to highlight variables, functions, classes and error informations)
    editor->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, FRD_INDIC_VARIABLE, QsciScintillaBase::INDIC_TEXTFORE);
    editor->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, FRD_INDIC_FUNCTION, QsciScintillaBase::INDIC_TEXTFORE);
    editor->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, FRD_INDIC_CLASS   , QsciScintillaBase::INDIC_TEXTFORE);
    editor->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, FRD_INDIC_ERROR   , QsciScintillaBase::INDIC_SQUIGGLE);
    editor->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, FRD_INDIC_WARNING , QsciScintillaBase::INDIC_SQUIGGLE);
    editor->SendScintilla(QsciScintillaBase::SCI_INDICSETSTYLE, FRD_INDIC_WARNING , QsciScintillaBase::INDIC_ROUNDBOX);
    editor->setIndicatorForegroundColor(0xff0000, FRD_INDIC_VARIABLE);
    editor->setIndicatorForegroundColor(0x17ccc6, FRD_INDIC_FUNCTION);
    editor->setIndicatorForegroundColor(0x0000ff, FRD_INDIC_CLASS);
    editor->setIndicatorForegroundColor(0xff0000, FRD_INDIC_ERROR);
    editor->setIndicatorForegroundColor(0x007f00, FRD_INDIC_WARNING);

    layout->addWidget(editor);

    connect(editor, SIGNAL(textChanged()), this, SLOT(waitToUpdateHighlight()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateHighlight()));
}

FRD_Editor::~FRD_Editor()
{
    delete editor;
    delete layout;
    delete timer;
}

QString FRD_Editor::text() const
{
    return editor->text();
}

void FRD_Editor::setText(const QString &text)
{
    editor->setText(text);
}

bool FRD_Editor::isEndString(QChar c) const
{
    if (c.isSpace()) return true;
    QString endString("!@#$%^&*-+=~`'\":;,.?()[]{}<>|\\");
    return endString.contains(c);
}

bool FRD_Editor::isComment(int pos) const
{
    int style = editor->SendScintilla(QsciScintillaBase::SCI_GETSTYLEAT, pos);
    return
            style == QsciLexerFRD::Comment                || style == QsciLexerFRD::InactiveComment                ||
            style == QsciLexerFRD::CommentDoc             || style == QsciLexerFRD::InactiveCommentDoc             ||
            style == QsciLexerFRD::CommentDocKeyword      || style == QsciLexerFRD::InactiveCommentDocKeyword      ||
            style == QsciLexerFRD::CommentDocKeywordError || style == QsciLexerFRD::InactiveCommentDocKeywordError ||
            style == QsciLexerFRD::CommentLine            || style == QsciLexerFRD::InactiveCommentLine            ||
            style == QsciLexerFRD::CommentLineDoc         || style == QsciLexerFRD::InactiveCommentLineDoc;
}

bool FRD_Editor::isString(int pos) const
{
    int style = editor->SendScintilla(QsciScintillaBase::SCI_GETSTYLEAT, pos);
    return
            style == QsciLexerFRD::SingleQuotedString         || style == QsciLexerFRD::InactiveSingleQuotedString         ||
            style == QsciLexerFRD::DoubleQuotedString         || style == QsciLexerFRD::InactiveDoubleQuotedString         ||
            style == QsciLexerFRD::TripleQuotedVerbatimString || style == QsciLexerFRD::InactiveTripleQuotedVerbatimString ||
            style == QsciLexerFRD::HashQuotedString           || style == QsciLexerFRD::InactiveHashQuotedString           ||
            style == QsciLexerFRD::RawString                  || style == QsciLexerFRD::InactiveRawString                  ||
            style == QsciLexerFRD::UnclosedString             || style == QsciLexerFRD::InactiveUnclosedString             ||
            style == QsciLexerFRD::VerbatimString             || style == QsciLexerFRD::InactiveVerbatimString;
}

void FRD_Editor::setIndicator(int start, int length, FRD_Indicator indicator)
{
    if (indicator < 0)
    {
        // clear all range between start and end
        if (length > 0) length = -length;
        setIndicator(start, length, FRD_INDIC_VARIABLE);
        setIndicator(start, length, FRD_INDIC_FUNCTION);
        setIndicator(start, length, FRD_INDIC_CLASS);
        setIndicator(start, length, FRD_INDIC_ERROR);
        setIndicator(start, length, FRD_INDIC_WARNING);
    }
    else
    {
        if (length < 0)
        {
            // clear range
            editor->SendScintilla(QsciScintillaBase::SCI_SETINDICATORCURRENT, indicator);
            editor->SendScintilla(QsciScintillaBase::SCI_INDICATORCLEARRANGE, start, -length);
        }
        else
        {
            // fill range
            editor->SendScintilla(QsciScintillaBase::SCI_SETINDICATORCURRENT, indicator);
            editor->SendScintilla(QsciScintillaBase::SCI_INDICATORFILLRANGE, start, length);
        }
    }
}

FRD_Editor::FRD_Indicator FRD_Editor::indicatorFromSymbol(QChar symbol) const
{
    if (symbol == '$') return FRD_INDIC_VARIABLE;
    else if (symbol == '%') return FRD_INDIC_FUNCTION;
    else if (symbol == '@') return FRD_INDIC_CLASS;
    else return FRD_INDIC_CLEAR;
}

void FRD_Editor::updateHighlight()
{
    timer->stop(); // Stop the timer since text has been updated
    auto text = editor->text(); // get text in the editor
    setIndicator(0, text.length(), FRD_INDIC_CLEAR); // clear all indicators
    for (int i = 0; i < text.length(); i++)
    {
        if (isComment(i) || isString(i)) continue;
        if (text[i] == '$' || text[i] == '%' || text[i] == '@')
        {
            int length = 0;
            while (i + (++length) < text.length() && !isEndString(text[i + length]));
            setIndicator(i, length, indicatorFromSymbol(text[i]));
            i = i + length - 1;
        }
    }
}

void FRD_Editor::waitToUpdateHighlight()
{
    emit textChanged();
    timer->start(waiting_time);
}
