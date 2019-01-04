//
// Created by 黄大举 on 2018/12/31.
//

#ifndef TUYAMEDIAPROJECT_FFMPEGMUSIC_H
#define TUYAMEDIAPROJECT_FFMPEGMUSIC_H

#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window_jni.h>
#include <unistd.h>

extern "C"{
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
//像素处理
#include "libswscale/swscale.h"
};

#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"LC",FORMAT,##__VA_ARGS__);

int createFFmpeg(int *rate, int *channel);

int getPcm(void **pcm, size_t *pcm_size);

void releaseFFmpeg();

#endif //TUYAMEDIAPROJECT_FFMPEGMUSIC_H
