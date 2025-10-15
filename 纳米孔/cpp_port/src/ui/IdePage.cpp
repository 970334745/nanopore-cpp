#include "IdePage.h"
#include <QSplitter>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QKeyEvent>
#include <QFontDatabase>
#include <QCompleter>
#include <QTextDocument>
#include <QTextCursor>
#include <QColor>
#include <QFileInfo>
#include <QStringList>

namespace {
// 简易代码编辑器，支持自动缩进与补全触发
class CodeEditor : public QPlainTextEdit {
public:
    explicit CodeEditor(QWidget* parent=nullptr) : QPlainTextEdit(parent) {
        QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        f.setPointSize(12);
        setFont(f);
    }
    void setCompleter(QCompleter* c) { completer_ = c; }
protected:
    void keyPressEvent(QKeyEvent* e) override {
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
            auto tc = textCursor();
            tc.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
            QString line = tc.selectedText();
            int spaces = 0;
            while (spaces < line.size() && line[spaces].isSpace()) spaces++;
            bool indentPlus = line.trimmed().endsWith(QLatin1Char(':'));
            QPlainTextEdit::keyPressEvent(e);
            insertPlainText(QString(spaces + (indentPlus ? 4 : 0), QLatin1Char(' ')));
            return;
        }
        if ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space) {
            if (completer_) completer_->complete();
            return;
        }
        QPlainTextEdit::keyPressEvent(e);
    }
private:
    QCompleter* completer_ = nullptr;
};

// 简易Python语法高亮
class PythonHighlighter : public QSyntaxHighlighter {
public:
    explicit PythonHighlighter(QTextDocument* doc) : QSyntaxHighlighter(doc) {
        QTextCharFormat kwFmt; kwFmt.setForeground(QColor("#7C4DFF")); kwFmt.setFontWeight(QFont::Bold);
        const QStringList keywords = {
            "False","class","finally","is","return","None","continue","for","lambda","try",
            "True","def","from","nonlocal","while","and","del","global","not","with",
            "as","elif","if","or","yield","assert","else","import","pass","raise"
        };
        for (const QString& kw : keywords) rules_.push_back({QRegularExpression("\\b" + kw + "\\b"), kwFmt});

        QTextCharFormat strFmt; strFmt.setForeground(QColor("#43A047"));
        rules_.push_back({QRegularExpression("\"([^\\\n]|\\.)*\""), strFmt});
        rules_.push_back({QRegularExpression("'([^\\\n]|\\.)*'"), strFmt});

        QTextCharFormat comFmt; comFmt.setForeground(QColor("#9E9E9E"));
        rules_.push_back({QRegularExpression("#.*"), comFmt});

        funcNameFmt_.setForeground(QColor("#039BE5")); funcNameFmt_.setFontWeight(QFont::DemiBold);
        funcNameRe_ = QRegularExpression("\\bdef\\s+(\\w+)");
    }
protected:
    void highlightBlock(const QString& text) override {
        for (const auto& r : rules_) {
            auto it = r.re.globalMatch(text);
            while (it.hasNext()) {
                auto m = it.next(); setFormat(m.capturedStart(), m.capturedLength(), r.fmt);
            }
        }
        auto m = funcNameRe_.match(text);
        if (m.hasMatch()) setFormat(m.capturedStart(1), m.capturedLength(1), funcNameFmt_);
    }
private:
    struct Rule { QRegularExpression re; QTextCharFormat fmt; };
    QVector<Rule> rules_;
    QRegularExpression funcNameRe_;
    QTextCharFormat funcNameFmt_;
};
}

IdePage::IdePage(QWidget* parent) : QWidget(parent), fsModel_(nullptr), tree_(nullptr), editor_(nullptr), console_(nullptr), toolbar_(nullptr), completer_(nullptr), process_(nullptr) {
    setupLayout();
    setupFileTree();
    setupEditorFeatures();
}

void IdePage::setupLayout() {
    auto* root = new QVBoxLayout(this);

    toolbar_ = new QToolBar(this);
    actNew_ = toolbar_->addAction(QStringLiteral("新建"));
    actOpen_ = toolbar_->addAction(QStringLiteral("打开"));
    actSave_ = toolbar_->addAction(QStringLiteral("保存"));
    actSaveAs_ = toolbar_->addAction(QStringLiteral("另存为"));
    toolbar_->addSeparator();
    actRun_ = toolbar_->addAction(QStringLiteral("运行"));
    actDebug_ = toolbar_->addAction(QStringLiteral("调试"));
    actStop_ = toolbar_->addAction(QStringLiteral("停止"));
    root->addWidget(toolbar_);

    auto* hsplit = new QSplitter(Qt::Horizontal, this);
    tree_ = new QTreeView(hsplit);
    auto* vsplit = new QSplitter(Qt::Vertical, hsplit);
    editor_ = new CodeEditor(vsplit);
    console_ = new QTextEdit(vsplit);
    console_->setReadOnly(true);
    vsplit->addWidget(editor_);
    vsplit->addWidget(console_);
    hsplit->addWidget(tree_);
    hsplit->addWidget(vsplit);
    hsplit->setStretchFactor(0, 1);
    hsplit->setStretchFactor(1, 3);
    root->addWidget(hsplit);

    connect(actNew_, &QAction::triggered, this, &IdePage::newFile);
    connect(actOpen_, &QAction::triggered, this, &IdePage::openFile);
    connect(actSave_, &QAction::triggered, this, &IdePage::saveFile);
    connect(actSaveAs_, &QAction::triggered, this, &IdePage::saveFileAs);
    connect(actRun_, &QAction::triggered, this, &IdePage::runScript);
    connect(actDebug_, &QAction::triggered, this, &IdePage::debugScript);
    connect(actStop_, &QAction::triggered, this, &IdePage::stopRun);
}

void IdePage::setupFileTree() {
    fsModel_ = new QFileSystemModel(this);
    fsModel_->setRootPath(QDir::currentPath());
    fsModel_->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    tree_->setModel(fsModel_);
    tree_->setRootIndex(fsModel_->index(QDir::currentPath()));
    tree_->setHeaderHidden(true);
    connect(tree_, &QTreeView::activated, this, &IdePage::onTreeActivated);
}

void IdePage::setupEditorFeatures() {
    // 语法高亮
    new PythonHighlighter(editor_->document());
    // 补全
    QStringList words = {
        "def","class","import","from","with","as","return","yield","for","while","if","elif","else",
        "try","except","finally","True","False","None","and","or","not","in","is","lambda","pass","break","continue"
    };
    completer_ = new QCompleter(words, this);
    completer_->setCaseSensitivity(Qt::CaseInsensitive);
    static_cast<CodeEditor*>(editor_)->setCompleter(completer_);
}

void IdePage::appendConsole(const QString& text) {
    console_->append(text);
}

void IdePage::newFile() {
    editor_->clear();
    currentFile_.clear();
}

void IdePage::openFile() {
    QString path = QFileDialog::getOpenFileName(this, QStringLiteral("打开文件"), QDir::currentPath(), QStringLiteral("Python文件 (*.py);;所有文件 (*.*)"));
    if (path.isEmpty()) return; loadFile(path);
}

void IdePage::onTreeActivated(const QModelIndex& idx) {
    QString path = fsModel_->filePath(idx);
    QFileInfo info(path);
    if (info.isFile()) loadFile(path);
}

void IdePage::loadFile(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("打开失败"), path);
        return;
    }
    QTextStream ts(&f);
    editor_->setPlainText(ts.readAll());
    currentFile_ = path;
}

void IdePage::saveFile() {
    if (currentFile_.isEmpty()) { saveFileAs(); return; }
    QFile f(currentFile_);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("保存失败"), currentFile_);
        return;
    }
    QTextStream ts(&f);
    ts << editor_->toPlainText();
    appendConsole(QStringLiteral("已保存: ") + currentFile_);
}

void IdePage::saveFileAs() {
    QString path = QFileDialog::getSaveFileName(this, QStringLiteral("另存为"), currentFile_.isEmpty() ? QDir::currentPath() + "/untitled.py" : currentFile_, QStringLiteral("Python文件 (*.py);;所有文件 (*.*)"));
    if (path.isEmpty()) return;
    currentFile_ = path; saveFile();
}

void IdePage::runScript() {
    if (process_) { appendConsole(QStringLiteral("已有进程在运行")); return; }
    if (currentFile_.isEmpty()) { appendConsole(QStringLiteral("请先保存文件")); saveFileAs(); if (currentFile_.isEmpty()) return; }
    process_ = new QProcess(this);
    connect(process_, &QProcess::readyReadStandardOutput, this, [this]{ appendConsole(QString::fromUtf8(process_->readAllStandardOutput())); });
    connect(process_, &QProcess::readyReadStandardError, this, [this]{ appendConsole(QString::fromUtf8(process_->readAllStandardError())); });
    connect(process_, &QProcess::finished, this, [this](int code){ appendConsole(QStringLiteral("进程退出: ") + QString::number(code)); process_->deleteLater(); process_ = nullptr; });
    QString program = QStringLiteral("python3");
    QStringList args = { QStringLiteral("-u"), currentFile_ };
    process_->start(program, args);
    appendConsole(QStringLiteral("运行: ") + program + " " + args.join(" "));
}

void IdePage::debugScript() {
    if (process_) { appendConsole(QStringLiteral("已有进程在运行")); return; }
    if (currentFile_.isEmpty()) { appendConsole(QStringLiteral("请先保存文件")); saveFileAs(); if (currentFile_.isEmpty()) return; }
    process_ = new QProcess(this);
    connect(process_, &QProcess::readyReadStandardOutput, this, [this]{ appendConsole(QString::fromUtf8(process_->readAllStandardOutput())); });
    connect(process_, &QProcess::readyReadStandardError, this, [this]{ appendConsole(QString::fromUtf8(process_->readAllStandardError())); });
    connect(process_, &QProcess::finished, this, [this](int code){ appendConsole(QStringLiteral("调试进程退出: ") + QString::number(code)); process_->deleteLater(); process_ = nullptr; });
    QString program = QStringLiteral("python3");
    QStringList args = { QStringLiteral("-m"), QStringLiteral("debugpy"), QStringLiteral("--listen"), QStringLiteral("5678"), QStringLiteral("--wait-for-client"), currentFile_ };
    process_->start(program, args);
    appendConsole(QStringLiteral("调试等待连接: 5678"));
}

void IdePage::stopRun() {
    if (!process_) return;
    process_->kill();
}