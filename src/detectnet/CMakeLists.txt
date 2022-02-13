find_package(OpenCV REQUIRED)

find_package(PkgConfig)
pkg_check_modules(Pistache REQUIRED IMPORTED_TARGET libpistache)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
)
set(LIBS
        ${OpenCV_LIBS}
        nadjieb_mjpeg_streamer::nadjieb_mjpeg_streamer
        tesseract
        nlohmann_json::nlohmann_json
        PkgConfig::Pistache
        )

file(GLOB detectnetSources *.cpp)
file(GLOB detectnetIncludes *.h )

file(GLOB codeIncludes "${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h")
file(GLOB codeSources "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")

cuda_add_executable(detectnet ${detectnetSources} ${codeSources})

target_link_libraries(detectnet jetson-inference ${LIBS})

install(TARGETS detectnet DESTINATION bin)

