#pragma once

#ifdef USE_QT5WEBKIT
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#else
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineCore/QWebEnginePage>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#endif

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

#ifdef USE_QT5WEBKIT

class Render
{
  public:
    Render(const char* const html, const options_t& options, results_t& results)
    {
        view_.resize(options.width, 10);
        view_.show();
        view_.setHtml(QString::fromUtf8(html));

        {
            QEventLoop loop;
            QObject::connect(&view_, &QWebView::loadFinished, &loop, &QEventLoop::quit);
            loop.exec();
        }

        QWebFrame* frame = view_.page()->mainFrame();
        const int doc_height = frame->contentsSize().height();

        view_.page()->setViewportSize(QSize(options.width, doc_height));

        QImage image(options.width, doc_height, QImage::Format_ARGB32);
        image.fill(Qt::white);
        QPainter painter(&image);
        frame->render(&painter);
        painter.end();

        image.save(options.filename);

        results.width = image.width();
        results.height = image.height();
    }

  private:
    QWebView view_;
};

#else

class Render
{
  public:
    Render(const char* const html, const options_t& options, results_t& results)
    {
        view_.resize(options.width, 10);
        view_.show();
        view_.setHtml(QString::fromUtf8(html));

        {
            QEventLoop loop;
            QObject::connect(view_.page(), &QWebEnginePage::loadFinished, &loop, &QEventLoop::quit);
            loop.exec();
        }

        // Get full document height via JavaScript
        int doc_height = 10;
        {
            QEventLoop loop;
            view_.page()->runJavaScript(
                QStringLiteral("Math.max(document.body.scrollHeight, document.documentElement.scrollHeight)"),
                [&](const QVariant& v) {
                    doc_height = v.toInt();
                    loop.quit();
                });
            loop.exec();
        }

        view_.resize(options.width, doc_height);

        // Wait for re-render after resize
        {
            QEventLoop loop;
            QTimer::singleShot(200, &loop, &QEventLoop::quit);
            loop.exec();
        }

#if defined(__APPLE__)
        // On macOS with cocoa platform, grab() works correctly
        QImage image = view_.grab().toImage();
#else
        // On Linux/Windows with offscreen platform, render() via QPainter
        QImage image(view_.size(), QImage::Format_ARGB32);
        image.fill(Qt::white);
        QPainter painter(&image);
        view_.render(&painter);
        painter.end();
#endif

        image.save(options.filename);

        results.width = image.width();
        results.height = image.height();
    }

  private:
    QWebEngineView view_;
};

#endif
