#include "RowTab.h"
#include "HiddenFirstTabBar.h"

#include <QTabBar>
#include <QMouseEvent>

RowTab::RowTab(uint rowNumber, QWidget *parent)
    : QTabWidget(parent)
    , m_rowNumber(rowNumber)
    , m_isActive(false)
{
    // Replace the default tab bar with one that hides tab 0.
    setTabBar(new HiddenFirstTabBar(this));

    tabBar()->setExpanding(false);
    tabBar()->setDrawBase(true);
    tabBar()->installEventFilter(this);

    connect(this, &QTabWidget::currentChanged,        this, &RowTab::onCurrentChanged);
    connect(this, &QTabWidget::tabBarClicked,         this, &RowTab::onTabBarClicked);
    connect(this, &QTabWidget::tabBarDoubleClicked,   this, &RowTab::onTabBarDoubleClicked);
    connect(this, &QTabWidget::tabCloseRequested,     this, &RowTab::onTabCloseRequested);
}

void RowTab::ensurePlaceholder()
{
    if (m_placeholderInstalled)
        return;

    // Insert an empty widget as tab 0. Because HiddenFirstTabBar reports
    // zero size for index 0, this tab is invisible and unclickable.
    QWidget *placeholder = new QWidget(this);
    QTabWidget::addTab(placeholder, QString());
    m_placeholderInstalled = true;

    // Make the placeholder current so nothing else shows by default.
    QTabWidget::setCurrentIndex(0);
}

int RowTab::addRealTab(QWidget *widget, const QString &title)
{
    ensurePlaceholder();
    return QTabWidget::addTab(widget, title);   // will be index >= 1
}

int RowTab::realTabCount() const
{
    return m_placeholderInstalled ? count() - 1 : 0;
}

void RowTab::activatePlaceholder()
{
    if (m_placeholderInstalled)
        QTabWidget::setCurrentIndex(0);
}

void RowTab::setActive(bool active)
{
    if (m_isActive == active)
        return;

    m_isActive = active;

    if (active) {
        setMaximumHeight(QWIDGETSIZE_MAX);
        setMinimumHeight(0);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    } else {
        const int tabHeight = tabBar()->sizeHint().height();
        setMaximumHeight(tabHeight);
        setMinimumHeight(tabHeight);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    updateGeometry();
}

void RowTab::setCurrentIndex(int index)
{
    QTabWidget::setCurrentIndex(index);
}

void RowTab::setCurrentWidget(QWidget *widget)
{
    QTabWidget::setCurrentWidget(widget);
}

bool RowTab::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == tabBar() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::LeftButton) {
            const int index = tabBar()->tabAt(me->pos());
            // Ignore clicks on the hidden placeholder (index 0).
            if (index > 0) {
                emit tabBarClicked(index);
            }
        }
    }
    return QTabWidget::eventFilter(obj, event);
}

void RowTab::onCurrentChanged(int index)      { emit currentChanged(index); }
void RowTab::onTabBarClicked(int index)       { emit tabBarClicked(index); }
void RowTab::onTabBarDoubleClicked(int index) { emit tabBarDoubleClicked(index); }
void RowTab::onTabCloseRequested(int index)   { emit tabCloseRequested(index); }
