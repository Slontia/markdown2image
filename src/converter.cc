#include "converter.h"

#include <qt5/QtCore/QObject>
#include <qt5/QtWebKitWidgets/QWebPage>
#include <qt5/QtWebKitWidgets/QWebFrame>
#include <qt5/QtGui/QPainter>
#include <qt5/QtGui/QImage>
#include <qt5/QtWidgets/QApplication>

#include "md4c-html.h"

#include "moc_render_image.cpp" // for AUTOMOC

struct value_t
{
    int ret;
    const options_t& options;
    results_t* results;
};

static void md4c_callback(const MD_CHAR* html, MD_SIZE html_size, void* p)
{
    static_cast<std::string*>(p)->append(html, html_size);
};

std::string markdown2html(const std::string& markdown)
{
    std::string output;
    output.reserve(markdown.size() + markdown.size() / 8 + 64);
    if (0 != md_html(markdown.c_str(), markdown.size(), &md4c_callback, &output, 0, MD_HTML_FLAG_DEBUG | MD_HTML_FLAG_SKIP_UTF8_BOM)) {
        return {};
    }
    return output;
}

void html2image(const std::string& html, const options_t& options, results_t* const results)
{
    static int argc = 0;
    static QApplication a(argc, nullptr); // could not use QCoreApplication because QWebPage will create Widget
    Render(html.c_str(), options, results);
}
