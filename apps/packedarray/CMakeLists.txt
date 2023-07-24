project(packed-array)

set(CMAKE_CXX_STANDARD 17)

# Set zserio version and fetch helper from Github
include(FetchContent)
set(ZSERIO_VERSION "2.11.0")

if (NOT TARGET zserio-cmake-helper)
  FetchContent_Declare(
    zserio-cmake-helper
    GIT_REPOSITORY https://github.com/Klebert-Engineering/zserio-cmake-helper.git
    GIT_TAG        "v1.0.0" # update if necessary
  )
  FetchContent_MakeAvailable(zserio-cmake-helper)
endif()

add_zserio_cpp_runtime()

# add zserio sources
add_zserio_library(packedzslib 
  ROOT ${CMAKE_CURRENT_SOURCE_DIR}/zs
  ENTRY packedarray.zs
)

# add main executable
add_executable(packed-array 
                main.cpp
                RolledArray.h)
target_link_libraries(packed-array PRIVATE packedzslib)
