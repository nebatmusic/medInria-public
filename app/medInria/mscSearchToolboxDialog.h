#ifndef mscSearchToolboxDialog_H
#define mscSearchToolboxDialog_H

#include <QDialog>
#include <QTreeWidget>

class QLineEdit;
class QPushButton;
class mscSearchToolboxDialogPrivate;

class mscSearchToolboxDialog : public QDialog
{
    Q_OBJECT

public:
    mscSearchToolboxDialog(QWidget *parent, QHash<QString, QStringList> toolboxDataHash);
    virtual ~mscSearchToolboxDialog();
    QStringList getFindText();

protected slots:
    /**
     * @brief searchTextChanged is called when the user write something in the search bar.
     * The according items are hidden and shown in the tree.
     */
    void searchTextChanged(const QString& text);

    /**
     * @brief searchItemDoubleClicked is called when the user double-click on an item.
     * The according workspace is opened.
     */
    void searchItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
    mscSearchToolboxDialogPrivate *d;
};
#endif
