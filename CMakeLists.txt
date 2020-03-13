cmake_minimum_required(VERSION 3.2)

project(proj VERSION 0.0.1)

# boost dependensies
find_package(OpenCV REQUIRED)

# source
set(SOURCE
        cv_utils.cpp cv_utils.h
        merge_lines.cpp merge_lines.h
)

set(EXE_SOURCE main.cpp ${SOURCE})

# targets and libraries
set(EXE_NAME search_tracks)
add_executable(${EXE_NAME} ${EXE_SOURCE})

# compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMP_OPTIONS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Woverloaded-virtual;-Wconversion;-Wsign-conversion;-Wduplicated-cond")
  if(CMAKE_BUILD_TYPE MATCHES Debug)
      #set(CMP_OPTIONS ${CMP_OPTIONS}";-D_GLIBCXX_DEBUG;-D_GLIBCXX_DEBUG_PEDANTIC")
      set(CMP_OPTIONS ${CMP_OPTIONS}";-fsanitize=address;")
      message("debug")
  endif()
else()
  set(CMP_OPTIONS "/W4")
endif()

# target properties
set_target_properties(${EXE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS ${CMP_OPTIONS}
)

# add headers
target_include_directories(${EXE_NAME}
        PRIVATE ${OpenCV_INCLUDE_DIR}
)

# target linking
target_link_libraries(${EXE_NAME}
        ${OpenCV_LIBRARIES}
)

# installation
install(TARGETS ${EXE_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT gwrass@mail.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
