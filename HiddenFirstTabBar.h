#ifndef HIDDENFIRSTTABBAR_H
#define HIDDENFIRSTTABBAR_H

#include <QTabBar>

// A QTabBar where tab index 0 is rendered with zero size.
// This lets us use tab 0 as a hidden "placeholder" tab so QTabWidget
// always has a valid current tab without displaying real content.
class HiddenFirstTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit HiddenFirstTabBar(QWidget *parent = nullptr) : QTabBar(parent) {}

protected:
    QSize tabSizeHint(int index) const override
    {
        if (index == 0)
            return QSize(0, 0);
        return QTabBar::tabSizeHint(index);
    }

    QSize minimumTabSizeHint(int index) const override
    {
        if (index == 0)
            return QSize(0, 0);
        return QTabBar::minimumTabSizeHint(index);
    }
};

#endif // HIDDENFIRSTTABBAR_H
