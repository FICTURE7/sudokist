# Check if we've got enough parameters.
if [ $# -lt 2 ]; then
    echo "usage: `basename $0` cmake_generator cmake_build_type" 1>&2
    exit 1
fi

# Directory of where the build files will be generated.
BUILD_DIR=build

# CMake generator we're going to use, argument 1.
GENERATOR=$1
# CMake build type we're going to use, argument 2.
BUILD_TYPE=$2

# Use CMake's command mode to create the directoy.
echo ".. Making ${BUILD_DIR} directory"
cmake -E make_directory "${BUILD_DIR}"

# Move into that folder and call cmake.
cd "${BUILD_DIR}"
cmake -D CMAKE_BUILD_TYPE="${BUILD_TYPE}" -G "${GENERATOR}" ../src
