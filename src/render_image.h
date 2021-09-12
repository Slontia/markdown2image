#pragma once

#include <future>

#include <qt5/QtCore/QObject>
#include <qt5/QtWebKitWidgets/QWebPage>
#include <qt5/QtWebKitWidgets/QWebFrame>
#include <qt5/QtGui/QPainter>
#include <qt5/QtGui/QImage>
#include <qt5/QtWidgets/QApplication>

#include <qt5/QtCore/QObject>

#include <qt5/QtTest/QSignalSpy>

#include "converter.h"

class Render : public QObject
{
    Q_OBJECT

  public:
    Render(const char* const html, const options_t& options, results_t* const results) : is_ok_(false)
    {
        QObject::connect(&page_, &QWebPage::loadFinished, this, &Render::finish);
        page_.setViewportSize(QSize(options.width, 10));
        page_.mainFrame()->setHtml(html);

        if (!is_ok_) { // if need run async
            QEventLoop loop;
            QObject::connect(&page_, &QWebPage::loadFinished, &loop, &QEventLoop::quit);
            loop.exec();
        }

        page_.setViewportSize(page_.mainFrame()->contentsSize());
        QImage image(page_.viewportSize(), QImage::Format_ARGB32);
        QPainter painter(&image);

        page_.mainFrame()->render(&painter);
        painter.end();

        image.save(options.filename);

        if (results) {
            results->height = page_.mainFrame()->contentsSize().height();
            results->width = page_.mainFrame()->contentsSize().width();
        }
    }

  public slots:
    void finish() { is_ok_ = true; }

  private:
    QWebPage page_;
    bool is_ok_;
};
