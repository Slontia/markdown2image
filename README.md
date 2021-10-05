# Markdown2Image

A tools to convert markdown to image.

## Dependence

1. Clone submodules (md4c)

        $ git submodule update --init --recursive

2. Install necessary packages (Qt5 Webkit)

        $ sudo yum install qt5-qtwebkit

## Usage

    markdown2image [--input=<markdown file path>] [--width=<max width of image>] [--output=<output image path>]

For example:

    ./markdown2image --input=../README.md --width=1000 --output=./readme.png

Or:

    cat ../README.md | ./markdown2image --width=1000 --output=./readme.png
