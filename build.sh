BUILD_DIR=build
BUILD_TYPE=$2
GENERATOR=$1

echo ".. Making ${BUILD_DIR} directory"
cmake -E make_directory "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake -D CMAKE_BUILD_TYPE="${BUILD_TYPE}" -G "${GENERATOR}" ../src
