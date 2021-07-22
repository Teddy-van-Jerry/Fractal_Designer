#include "FRD_Editor.h"

FRD_Editor::FRD_Editor(QLayout* layout_)
{
    editor = new QsciScintilla(this);
    layout = layout_;

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

    layout->addWidget(editor);

    connect(editor, SIGNAL(textChanged()), this, SLOT(updateHighlight()));
}
