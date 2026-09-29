#ifndef ROWTAB_H
#define ROWTAB_H

#include <QTabWidget>

class RowTab : public QTabWidget
{
    Q_OBJECT

public:
    explicit RowTab(uint rowNumber, QWidget *parent = nullptr);

    uint rowNumber() const { return m_rowNumber; }

    bool isActive() const { return m_isActive; }
    void setActive(bool active);
    void applyActiveGeometry(bool active);

    int addRealTab(QWidget *widget, const QString &title);
    int realTabCount() const;
    void activatePlaceholder();

signals:
    void currentChanged(int index);
    void tabBarClicked(int index);
    void tabBarDoubleClicked(int index);
    void tabCloseRequested(int index);

public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onCurrentChanged(int index);
    void onTabBarClicked(int index);
    void onTabBarDoubleClicked(int index);
    void onTabCloseRequested(int index);

private:
    void ensurePlaceholder();
    int  tabBarMinHeight() const;   // safe fallback for early calls

    uint m_rowNumber;
    bool m_isActive;
    bool m_placeholderInstalled = false;
    bool m_initialGeometryApplied = false;
};

#endif // ROWTAB_H