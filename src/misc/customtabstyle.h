#ifndef CUSTOMTABSTYLE_H
#define CUSTOMTABSTYLE_H

#include <QProxyStyle>

class CustomTabStyle : public QProxyStyle
{
public:
    CustomTabStyle();

    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
            const QSize &size, const QWidget *widget) const;

    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const;
};

#endif // CUSTOMTABSTYLE_H
