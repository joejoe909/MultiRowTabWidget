#include "RowTab.h"
#include "HiddenFirstTabBar.h"

#include <QTabBar>
#include <QMouseEvent>
#include <QShowEvent>

RowTab::RowTab(uint rowNumber, QWidget *parent)
    : QTabWidget(parent)
    , m_rowNumber(rowNumber)
    , m_isActive(false)
{
    setTabBar(new HiddenFirstTabBar(this));

    tabBar()->setExpanding(false);
    tabBar()->setDrawBase(true);
    tabBar()->installEventFilter(this);

    // Install the hidden placeholder tab up front so the tab bar has a real,
    // non-empty size hint even before any user tabs are added.
    ensurePlaceholder();

    connect(this, &QTabWidget::currentChanged,      this, &RowTab::onCurrentChanged);
    connect(this, &QTabWidget::tabBarClicked,       this, &RowTab::onTabBarClicked);
    connect(this, &QTabWidget::tabBarDoubleClicked, this, &RowTab::onTabBarDoubleClicked);
    connect(this, &QTabWidget::tabCloseRequested,   this, &RowTab::onTabCloseRequested);
}

void RowTab::ensurePlaceholder()
{
    if (m_placeholderInstalled)
        return;

    QWidget *placeholder = new QWidget(this);
    QTabWidget::addTab(placeholder, QString());
    m_placeholderInstalled = true;
    QTabWidget::setCurrentIndex(0);
}

int RowTab::addRealTab(QWidget *widget, const QString &title)
{
    ensurePlaceholder();   // no-op after ctor, kept for safety
    return QTabWidget::addTab(widget, title);
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

// Returns a reliable tab-bar height even when called very early, before the
// tab bar has been laid out. The fallback matches the default Qt style height.
int RowTab::tabBarMinHeight() const
{
    const int h = tabBar()->sizeHint().height();
    return (h > 0) ? h : 26;   // 26px is a safe default for most styles
}

void RowTab::applyActiveGeometry(bool active)
{
    if (active) {
        setMinimumHeight(0);
        setMaximumHeight(QWIDGETSIZE_MAX);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    } else {
        const int h = tabBarMinHeight();
        setMinimumHeight(h);
        setMaximumHeight(h);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
    updateGeometry();
}

void RowTab::setActive(bool active)
{
    if (m_isActive == active)
        return;

    m_isActive = active;
    applyActiveGeometry(active);
}

// Once the widget is actually shown, the tab bar has a real size hint.
// Re-apply the current state so inactive rows get the exact correct height.
void RowTab::showEvent(QShowEvent *event)
{
    QTabWidget::showEvent(event);

    if (!m_initialGeometryApplied) {
        m_initialGeometryApplied = true;
        applyActiveGeometry(m_isActive);
    }
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
            if (index > 0)
                emit tabBarClicked(index);
        }
    }
    return QTabWidget::eventFilter(obj, event);
}

void RowTab::onCurrentChanged(int index)      { emit currentChanged(index); }
void RowTab::onTabBarClicked(int index)       { emit tabBarClicked(index); }
void RowTab::onTabBarDoubleClicked(int index) { emit tabBarDoubleClicked(index); }
void RowTab::onTabCloseRequested(int index)   { emit tabCloseRequested(index); }