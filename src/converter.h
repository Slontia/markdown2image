#pragma once

#include <iostream>

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

extern "C" {

int markdown2image(const char* const markdown, const uint64_t markdown_size, const options_t* const options, results_t* const results);

}
