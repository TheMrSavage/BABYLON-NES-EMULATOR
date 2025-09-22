FetchContent_Declare(
  nativefiledialog
  GIT_REPOSITORY https://github.com/mlabbe/nativefiledialog.git
  GIT_TAG release_116
)
FetchContent_MakeAvailable(nativefiledialog)

add_library(nativefiledialog
    ${nativefiledialog_SOURCE_DIR}/src/nfd_common.c
    ${nativefiledialog_SOURCE_DIR}/src/nfd_zenity.c
)

target_include_directories(nativefiledialog PUBLIC ${nativefiledialog_SOURCE_DIR}/src/include)
