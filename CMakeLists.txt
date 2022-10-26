cmake_minimum_required(VERSION 3.16)
project(sndfile-example)

SET (CMAKE_BUILD_TYPE "Release")
#SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
SET (CMAKE_CXX_FLAGS_RELEASE "-O2")
SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET (BASE_DIR ${CMAKE_SOURCE_DIR} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/build)

add_executable (wav_cp wav_cp.cpp)
target_link_libraries (wav_cp sndfile)

add_executable (wav_hist wav_hist.cpp)
target_link_libraries (wav_hist sndfile)

add_executable (wav_dct wav_dct.cpp)
target_link_libraries (wav_dct sndfile fftw3)

add_executable (wav_dct_codec wav_dct_codec.cpp)
target_link_libraries (wav_dct_codec sndfile fftw3)

add_executable (wav_quant wav_quant.cpp)
target_link_libraries (wav_quant sndfile)

add_executable (wav_quant_v1 wav_quant_v1.cpp)
target_link_libraries (wav_quant_v1 sndfile)

add_executable(text_codec text_codec.cpp)

add_executable(wav_cmp wav_cmp.cpp)

add_executable(wav_cmp2 wav_cmp1.cpp)
target_link_libraries (wav_cmp2 sndfile fftw3)

add_executable(wav_effects wav_effects.cpp)

add_executable(bitstream_test BitStreamTest.cpp)
