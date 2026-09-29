#include "MultiRowTabWidget.h"
#include "RowTab.h"

#include <QVBoxLayout>

MultiRowTabWidget::MultiRowTabWidget(uint rows, QWidget *parent)
    : QWidget(parent)
    , m_maxRows(rows)
    , m_layout(new QVBoxLayout(this))
    , m_activeRow(0)
    , m_hasActiveRow(false)
{
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    createRows();

    if (m_maxRows > 0)
        makeActiveRow(0);
}

void MultiRowTabWidget::createRows()
{
    for (uint i = 0; i < m_maxRows; ++i) {
        RowTab *rowTab = new RowTab(i, this);

        connect(rowTab, &RowTab::tabBarClicked, this,
                [this, i](int /*index*/) {
                    onTabBarClicked(i, true);
                });

        m_rowTabs.insert(i, rowTab);
        m_layout->addWidget(rowTab);
    }
}

int MultiRowTabWidget::addTab(QWidget *widget, const QString &title, uint row)
{
    RowTab *rowTab = m_rowTabs.value(row, nullptr);
    if (!rowTab)
        return -1;

    // addRealTab internally ensures the hidden placeholder exists.
    const int index = rowTab->addRealTab(widget, title);

    // First real tab for this row: make it current.
    if (rowTab->realTabCount() == 1) {
        rowTab->setCurrentIndex(1);   // index 1 is the first real tab
    }

    return index;
}

RowTab *MultiRowTabWidget::rowTab(uint row) const
{
    return m_rowTabs.value(row, nullptr);
}

void MultiRowTabWidget::onTabBarClicked(uint active_row, bool is_active)
{
    if (is_active)
        makeActiveRow(active_row);
}

void MultiRowTabWidget::makeActiveRow(uint row)
{
    RowTab *target = m_rowTabs.value(row, nullptr);
    if (!target)
        return;

    // Deactivate all other rows AND reset them to their hidden placeholder tab,
    // so their real content disappears.
    for (auto it = m_rowTabs.constBegin(); it != m_rowTabs.constEnd(); ++it) {
        if (it.key() == row)
            continue;
        RowTab *rt = it.value();
        rt->setActive(false);
        rt->activatePlaceholder();   // <-- tab 0 (hidden) becomes current
    }

    // Activate the target row
    target->setActive(true);

    // Move the active row to the bottom of the layout.
    m_layout->removeWidget(target);
    m_layout->addWidget(target);

    // If the target row is still showing the placeholder (no real tab yet),
    // nothing more to do. Otherwise, make sure the clicked tab is current.
    // (The click handler in QTabWidget will already have set the current index
    //  to the clicked tab thanks to our eventFilter workaround.)
    if (target->realTabCount() > 0 && target->currentIndex() < 1)
        target->setCurrentIndex(1);   // fall back to first real tab

    m_activeRow = row;
    m_hasActiveRow = true;
}

void MultiRowTabWidget::makeRowInactive()
{
    if (m_hasActiveRow)
        makeActiveRow(m_activeRow);
}
