#include <fstream>
#include <iostream>
#include <cstring>

#include <gflags/gflags.h>
#include <unistd.h>

#include <md4c-html.h>

#include "render_image.h"
#include "moc_render_image.cpp" // for AUTOMOC

DEFINE_string(input, "", "The input filename of the markdown file");
DEFINE_string(output, "output.png", "The output filename of the image");
DEFINE_int32(width, 700, "The width of the image");

namespace {

bool valid_string(const char* flag_name, const std::string& str)
{
    if (!str.empty()) {
        return true;
    }
    std::cerr << "[ERROR] Empty flag \"" << flag_name << "\"" << std::endl;
    return false;
}

bool valid_positive_number(const char* flag_name, const int32_t n)
{
    if (n > 0) {
        return true;
    }
    std::cerr << "[ERROR] Non-Positive flag \"" << flag_name << "\"" << std::endl;
    return false;
}

const bool validate_output = google::RegisterFlagValidator(&FLAGS_output, &valid_string);
const bool validate_width = google::RegisterFlagValidator(&FLAGS_width, &valid_positive_number);

std::string load_markdown()
{
    const auto read = [](std::istream& is) {
        std::string s;
        for (std::string line; std::getline(is, line); ) {
            s += std::move(line) + "\n";
        }
        return s;
    };

    if (FLAGS_input.empty()) {
        return read(std::cin);
    }

    std::ifstream f(FLAGS_input);
    if (!f) {
        std::cerr << "[ERROR] Cannot open file " << FLAGS_input << ", msg: " << std::strerror(errno) << std::endl;
        return {};
    }

    return read(f);
}

void md4c_callback(const MD_CHAR* html, MD_SIZE html_size, void* p)
{
    static_cast<std::string*>(p)->append(html, html_size);
};

std::string markdown2html(const std::string& markdown)
{
    std::string output;
    output.reserve(markdown.size() + markdown.size() / 8 + 64);
    if (0 != md_html(markdown.c_str(), markdown.size(), &md4c_callback, &output, MD_FLAG_TABLES,
                MD_HTML_FLAG_DEBUG | MD_HTML_FLAG_SKIP_UTF8_BOM)) {
        return {};
    }
    return output;
}

int html2image(const char* html, const options_t& options, results_t& results)
{
    int argc = 0;
    QApplication a(argc, nullptr); // could not use QCoreApplication because QWebPage will create Widget
    Render(html, options, results);
    return 0;
}

}

const char* const k_table_css =
"<style>\n"
"table {\n"
"  border-collapse: collapse;\n"
"}\n"
"\n"
"table, th, td {\n"
"  border: 1px solid black;\n"
"}\n"
"</style>\n";

int main(int argc, char** argv)
{
    setenv("QT_QPA_PLATFORM", "offscreen", 1);

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    options_t options;
    options.filename = FLAGS_output.c_str();
    options.width = static_cast<uint32_t>(FLAGS_width);

    results_t results;

    const std::string markdown = load_markdown();
    if (markdown.empty()) {
        return -1;
    }

    const std::string html = markdown2html(markdown);
    if (html.empty()) {
        std::cerr << "[ERROR] Failed to convert to html" << std::endl;
        return -1;
    }
    const auto ret = html2image((k_table_css + html).c_str(), options, results);
    if (0 != ret) {
        std::cerr << "[ERROR] Failed to convert to image" << std::endl;
    }
    return ret;

    std::cout << "Generate image to " << FLAGS_output << ", size=(" << results.width << ", " << results.height << ")" << std::endl;

    return 0;
}

