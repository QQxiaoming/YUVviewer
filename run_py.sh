#!/bin/sh

git describe --always --long --dirty --abbrev=10 --tags | awk '{printf ""$0""}' > git_tag.inc
python ./src/YUVviewer.py
