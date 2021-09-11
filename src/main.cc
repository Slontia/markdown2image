#include "converter.h"

#include "converter.h"

#include <fstream>
#include <iostream>
#include <cstring>

#include <gflags/gflags.h>

#include "render_image.h"

#include <qt5/QtCore/QFile>

DEFINE_string(input, "", "The input filename of the markdown file");
DEFINE_string(output, "output.png", "The output filename of the image");
DEFINE_int32(width, 700, "The width of the image");

std::string markdown2html(const std::string& markdown);
int html2image(const std::string& html, const options_t& options, results_t* const results);

static bool valid_string(const char* flag_name, const std::string& str)
{
    if (!str.empty()) {
        return true;
    }
    std::cerr << "[ERROR] Empty flag \"" << flag_name << "\"" << std::endl;
    return false;
}

static bool valid_positive_number(const char* flag_name, const int32_t n)
{
    if (n > 0) {
        return true;
    }
    std::cerr << "[ERROR] Non-Positive flag \"" << flag_name << "\"" << std::endl;
    return false;
}

static const bool validate_input = google::RegisterFlagValidator(&FLAGS_input, &valid_string);
static const bool validate_output = google::RegisterFlagValidator(&FLAGS_input, &valid_string);
static const bool validate_width = google::RegisterFlagValidator(&FLAGS_width, &valid_positive_number);

static std::string load_file(const std::string& filename)
{
    std::ifstream f(FLAGS_input);
    if (!f) {
        std::cerr << "[ERROR] Cannot open file " << FLAGS_input << ", msg: " << std::strerror(errno) << std::endl;
        return {};
    }

    std::string s;
    for (std::string line; std::getline(f, line); ) {
        s += std::move(line) + "\n";
    }

    return s;
}

int main(int argc, char** argv)
{
    setenv("QT_QPA_PLATFORM", "offscreen", 1);

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    const std::string markdown = load_file(FLAGS_input);
    if (markdown.empty()) {
        return -1;
    }

    const std::string html = markdown2html(markdown);
    if (html.empty()) {
        std::cerr << "[ERROR] Failed to convert to html" << std::endl;
        return -1;
    }

    options_t options;
    options.filename = FLAGS_output.c_str();
    options.width = static_cast<uint32_t>(FLAGS_width);

    results_t results;

    const auto ret = html2image(html.c_str(), options, &results);
    if (0 != ret) {
        return ret;
    }

    std::cout << "Generate image to " << FLAGS_output << ", size=(" << results.width << ", " << results.height << ")" << std::endl;

    return 0;
}
