#pragma once

#include <iostream>

#include <qt5/QtCore/QObject>
#include <qt5/QtWebKitWidgets/QWebPage>
#include <qt5/QtWebKitWidgets/QWebFrame>
#include <qt5/QtGui/QPainter>
#include <qt5/QtGui/QImage>
#include <qt5/QtWidgets/QApplication>

class Converter : public QObject
{
    Q_OBJECT

public:
    Converter(const char* html)
    {
        page.mainFrame()->setHtml(html);
        page.setViewportSize(QSize(1000, 1000));
        //page.mainFrame()->load(QUrl("http://www.baidu.com"));
        connect(&page, SIGNAL(loadFinished(bool)), this, SLOT(render()));
    }

    ~Converter()
    {
    }

Q_SIGNALS:
    void finished();

private Q_SLOTS:
    void render()
    {
        std::cout
            << "size: "
            << page.mainFrame()->contentsSize().height()
            << " "
            << page.mainFrame()->contentsSize().width()
            << std::endl;
        page.setViewportSize(page.mainFrame()->contentsSize());
        QImage image(page.viewportSize(), QImage::Format_ARGB32);
        QPainter painter(&image);

        page.mainFrame()->render(&painter);
        painter.end();

        QImage thumbnail = image.scaled(400, 400);
        thumbnail.save("thumbnail.png");
        QApplication::quit();
        //emit finished();
    }

private:
    QWebPage page;
};
