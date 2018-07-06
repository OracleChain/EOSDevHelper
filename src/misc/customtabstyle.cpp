#include "customtabstyle.h"
#include <QStyleOptionTab>
#include <QPainter>

CustomTabStyle::CustomTabStyle()
{

}

QSize CustomTabStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    auto s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
        s.transpose();
        s.rwidth() = 248;
        s.rheight() = 42;
    }
    return s;
}

void CustomTabStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == CE_TabBarTabLabel) {
        if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
            auto allRect = tab->rect;
            if (tab->state & QStyle::State_Selected) {
                painter->save();
                painter->setBrush(QBrush(0x000001));
                painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                painter->restore();
            }

            QTextOption option;
            option.setAlignment(Qt::AlignCenter);
            painter->drawText(allRect, tab->text, option);
            return;
        }
    }

    if (element == CE_TabBarTab) {
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}
