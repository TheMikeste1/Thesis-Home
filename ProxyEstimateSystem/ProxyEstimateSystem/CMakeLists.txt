# CMakeList.txt : CMake project for ProxyEstimateSystem, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Get Arrow to create arrow files
# find_package(Arrow CONFIG REQUIRED)  # TODO: Uncomment this line when Arrow we start using Arrow

# Add source to this project's executable.
add_executable (ProxyEstimateSystem "main.cpp"  "TruthEstimator.h" "Properties.h" "Rankings.h" "Utilities.h"  "Rankings.cpp")
# target_link_libraries(ProxyEstimateSystem PRIVATE arrow_shared)  # TODO: Uncomment this line when Arrow we start using Arrow

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ProxyEstimateSystem PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
