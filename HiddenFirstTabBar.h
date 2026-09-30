#ifndef HIDDENFIRSTTABBAR_H
#define HIDDENFIRSTTABBAR_H

#include <QTabBar>
#include <QStyleOptionTab>
#include <QPainter>
#include <QStylePainter>

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

    // The stylesheet sets min-width and padding on QTabBar::tab, which the
    // style applies to every tab. To hide index 0 completely we skip painting
    // it altogether instead of letting the style draw a 100px sliver.
    void paintEvent(QPaintEvent *event) override
    {
        if (count() > 0) {
            QStylePainter painter(this);
            for (int i = 0; i < count(); ++i) {
                if (i == 0)
                    continue;              // skip the placeholder entirely
                QStyleOptionTab opt;
                initStyleOption(&opt, i);
                painter.drawControl(QStyle::CE_TabBarTab, opt);
            }
            return;
        }
        QTabBar::paintEvent(event);
    }
};


#endif // HIDDENFIRSTTABBAR_H
