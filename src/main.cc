#include "converter.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

#include "moc_converter.cpp" // for Qt AUTOMOC

int main(int argc, char *argv[])
{
    setenv("QT_QPA_PLATFORM", "offscreen", 1);

    QApplication a(argc, argv); // could not use QCoreApplication because QWebPage will create Widget

    std::string html;
    std::ifstream f("/home/bjcwgqm/demo.html");
    for (std::string line; std::getline(f, line); ) {
        html += std::move(line);
    }

    Converter w(html.c_str());

    return a.exec();
}
