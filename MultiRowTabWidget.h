#ifndef MULTIROWTABWIDGET_H
#define MULTIROWTABWIDGET_H

#include <QWidget>
#include <QHash>

class QVBoxLayout;
class RowTab;

class MultiRowTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultiRowTabWidget(uint rows, QWidget *parent = nullptr);

    int addTab(QWidget *widget, const QString &title, uint row);

    RowTab *rowTab(uint row) const;
    uint rows() const { return m_maxRows; }

public slots:
    void onTabBarClicked(uint active_row, bool is_active = true);

private:
    void createRows();
    void makeActiveRow(uint row);
    void makeRowInactive();

    uint m_maxRows;
    QVBoxLayout *m_layout;
    QHash<uint, RowTab *> m_rowTabs;
    uint m_activeRow;
    bool m_hasActiveRow;
};

#endif // MULTIROWTABWIDGET_H
