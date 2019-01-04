# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(libs/include)
include_directories(src/main/cpp/include)
set(DIR ../../../../libs)
add_library(avcodec-v7a
            SHARED
            IMPORTED)
set_target_properties(avcodec-v7a
                      PROPERTIES IMPORTED_LOCATION
                      ${DIR}/armeabi-v7a/libavcodec.so)

add_library(avdevice-v7a
            SHARED
            IMPORTED)
set_target_properties(avdevice-v7a
                      PROPERTIES IMPORTED_LOCATION
                      ${DIR}/armeabi-v7a/libavdevice.so)
add_library(avformat-v7a
            SHARED
            IMPORTED)
set_target_properties(avformat-v7a
                      PROPERTIES IMPORTED_LOCATION
                      ${DIR}/armeabi-v7a/libavformat.so)
add_library(avutil-v7a
            SHARED
            IMPORTED)
set_target_properties(avutil-v7a
                      PROPERTIES IMPORTED_LOCATION
                      ${DIR}/armeabi-v7a/libavutil.so)

add_library(swresample-v7a
             SHARED
             IMPORTED)
set_target_properties(swresample-v7a
                      PROPERTIES IMPORTED_LOCATION
                      ${DIR}/armeabi-v7a/libswresample.so)
  add_library(swscale-v7a
              SHARED
              IMPORTED)
  set_target_properties(swscale-v7a
                        PROPERTIES IMPORTED_LOCATION
                        ${DIR}/armeabi-v7a/libswscale.so)
  add_library(avfilter-v7a
              SHARED
              IMPORTED)
  set_target_properties(avfilter-v7a
                        PROPERTIES IMPORTED_LOCATION
                        ${DIR}/armeabi-v7a/libavfilter.so)

  add_library( # Sets the name of the library.
               native-lib

               # Sets the library as a shared library.
               SHARED

               # Provides a relative path to your source file(s).
               src/main/cpp/native-lib.cpp
               src/main/cpp/FFmpegMusic.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                        native-lib
                        avfilter-v7a
                        avcodec-v7a
                        avdevice-v7a
                        avformat-v7a
                        avutil-v7a
                        swresample-v7a
                        swscale-v7a
                        -landroid
                        OpenSLES
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )