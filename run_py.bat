@echo off

git describe --always --long --dirty --abbrev=10 --tags | ./tools/sed/awk.exe '{print "\""$0"\""}' > git_tag.inc
python .\src\YUVviewer.py
