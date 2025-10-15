#pragma once
#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtCore/QModelIndex>
#include <QtCore/QString>

class QTreeView;
class QFileSystemModel;
class QPlainTextEdit;
class QTextEdit;
class QToolBar;
class QAction;
class QCompleter;
class QProcess;

class IdePage : public QWidget {
    Q_OBJECT
public:
    explicit IdePage(QWidget* parent = nullptr);

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void runScript();
    void debugScript();
    void stopRun();
    void onTreeActivated(const QModelIndex& idx);

private:
    QFileSystemModel* fsModel_;
    QTreeView* tree_;
    QPlainTextEdit* editor_;
    QTextEdit* console_;
    QToolBar* toolbar_;

    QAction* actNew_;
    QAction* actOpen_;
    QAction* actSave_;
    QAction* actSaveAs_;
    QAction* actRun_;
    QAction* actDebug_;
    QAction* actStop_;

    QCompleter* completer_;
    QString currentFile_;
    QProcess* process_;

    void setupLayout();
    void setupEditorFeatures();
    void setupFileTree();
    void loadFile(const QString& path);
    void appendConsole(const QString& text);
};