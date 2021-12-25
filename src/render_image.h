#pragma once

#include <future>

#include <QtCore/QObject>
#include <QtWebKitWidgets/QWebPage>
#include <QtWebKitWidgets/QWebFrame>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtWidgets/QApplication>

struct options_t
{
    const char* filename = nullptr;
    uint32_t width = 700;
};

struct results_t
{
    uint32_t width = 0;
    uint32_t height = 0;
};

class Render : public QObject
{
    Q_OBJECT

  public:
    Render(const char* const html, const options_t& options, results_t& results) : is_ok_(false)
    {
        QObject::connect(&page_, &QWebPage::loadFinished, this, &Render::finish);
        page_.setViewportSize(QSize(options.width, 10));
        page_.mainFrame()->setHtml(html);

        if (!is_ok_) { // if need run async
            QEventLoop loop;
            // not wait loadFinished to avoid finish before exec
            QObject::connect(this, &Render::over, &loop, &QEventLoop::quit);
            loop.exec();
        }

        page_.setViewportSize(page_.mainFrame()->contentsSize());
        QImage image(page_.viewportSize(), QImage::Format_ARGB32);
        QPainter painter(&image);

        page_.mainFrame()->render(&painter);
        painter.end();

        image.save(options.filename);

        results.height = page_.mainFrame()->contentsSize().height();
        results.width = page_.mainFrame()->contentsSize().width();
    }

  public slots:
    void finish()
    {
        is_ok_ = true;
        emit over();
    }

  signals:
    void over();

  private:
    QWebPage page_;
    bool is_ok_;
};
