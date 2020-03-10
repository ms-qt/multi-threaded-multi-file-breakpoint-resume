find . -name "*.h" -exec vim -c "set encoding=utf-8 " -c wq! {} \;
find . -name "*.cpp" -exec vim -c "set encoding=utf-8 " -c wq! {} \;
find . -name "*.h" -exec vim -c "set bomb " -c wq! {} \;
find . -name "*.cpp" -exec vim -c "set bomb " -c wq! {} \;