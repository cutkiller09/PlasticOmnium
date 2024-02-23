#!/bin/sh 

# Copy - Paste all *.gcno and *.gcda fils in Unit and Integration tests folders
cp -rf build/CMakeFiles/TU_$COMPONENT.dir/test/01_SWE4_Units_Tests/src/* test/01_SWE4_Units_Tests/doc/coverage
cp -rf build/CMakeFiles/TU_$COMPONENT.dir/src/*.gc* test/01_SWE4_Units_Tests/doc/coverage

# Copy tests results report into $COMPONENT Integration tests and TU_$COMPONENT Unit tests doc folder
cp build/*TU_$COMPONENT*.md test/01_SWE4_Units_Tests/doc/

# Unit tests Coverage Report
#cd -
cd test/01_SWE4_Units_Tests/doc/coverage

# Generate reports (.gcov files). All .gcda files in the build directory are passed as gcov args.
find . -name '*.gcda' | xargs gcov

sleep 2

# 1. Generate the coverage.info data file
lcov --capture --directory . --output-file coverage.info

# 2. Generate a report from this data file
genhtml coverage.info --output-directory out

# 3. Remove .gcov and .gcda and .info files
rm -rf *.gcno *.gcov *.gcda *.info *.obj *.d 

# 4. mv out files in coverage root folder
cp -rf out/* ./
rm -rf out/

