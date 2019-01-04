//
// Created by 黄大举 on 2018/12/31.
//

#include "include/FFmpegMusic.h"

AVFormatContext *pFormatCtx;
AVCodecParameters *pCodecCtx;
AVCodec *pCodec;
AVCodecContext *codecContext;
AVPacket *pPacket;
AVFrame *frame;
SwrContext *swrCtx;
uint8_t *out_buffer;
int out_channel_nb;
int audio_stream_idx = -1;


int createFFmpeg(int *rate, int *channel) {
    //注册解码器，以及相关协议
    av_register_all();
    char *input = "/sdcard/Camera/VID_20181229_193743.mp4";
    pFormatCtx = avformat_alloc_context();
    LOGE("path %s", input);
    LOGE("xxx %p", pFormatCtx);
    char buf[] = "";
    int open_state = avformat_open_input(&pFormatCtx, input, NULL, NULL);
    if (open_state < 0) {
        av_strerror(open_state, buf, 1024);
        // LOGE("%s" ,inputPath)
        LOGE("Couldn't open file %s: %d(%s)", input, open_state, buf);
        // LOGE("%d",error)
        LOGE("打开视频失败")
    }

    if (avformat_find_stream_info(pFormatCtx, NULL)) {
        LOGE("读取输入的视频流信息失败。");
        return -1;
    }

    int i = 0;
    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_idx = i;
            break;
        }
    }

    if (audio_stream_idx == -1) {
        LOGE("没有找到音频流");
        return -1;
    }

    //获取音频编解码器
    pCodecCtx = pFormatCtx->streams[audio_stream_idx]->codecpar;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

    if (pCodec == NULL) {
        LOGE("未找到对应的流解码器。");
        return -1;
    }
    //通过解码器分配(并用  默认值   初始化)一个解码器context
    codecContext = avcodec_alloc_context3(pCodec);
    if (codecContext == NULL) {
        LOGE("分配 解码器上下文失败。");
        return -1;
    }
    //更具指定的编码器值填充编码器上下文
    if (avcodec_parameters_to_context(codecContext, pCodecCtx) < 0) {
        LOGE("填充编解码器上下文失败。");
        return -1;
    }
    //通过所给的编解码器初始化编解码器上下文
    if (avcodec_open2(codecContext, pCodec, NULL) < 0) {
        LOGE("初始化 解码器上下文失败。");
        return -1;
    }

    //申请avpakcet，装解码前的数据
    pPacket = av_packet_alloc();
    //申请avframe，装解码后的数据
    frame = av_frame_alloc();
    //mp3里面所包含的编码格式转换成pcm SwcContext
    swrCtx = swr_alloc();
    int length=0;
    int got_frame;
    //    44100*2
    out_buffer = (uint8_t *) av_malloc(44100 * 2);
    uint64_t  out_ch_layout=AV_CH_LAYOUT_STEREO;
    //    输出采样位数  16位
    enum AVSampleFormat out_formart=AV_SAMPLE_FMT_S16;
//输出的采样率必须与输入相同
    int out_sample_rate = pCodecCtx->sample_rate;
    swr_alloc_set_opts(swrCtx, out_ch_layout, out_formart, out_sample_rate,
                       pCodecCtx->channel_layout, codecContext->sample_fmt, pCodecCtx->sample_rate, 0,
                       NULL);
    swr_init(swrCtx);
    //    获取通道数  2
    out_channel_nb = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    *rate = pCodecCtx->sample_rate;
    *channel = pCodecCtx->channels;
    return 0;
}

int getPcm(void **pcm, size_t *pcm_size) {
    int frame_count = 0;
    int got_frame;
    while(av_read_frame(pFormatCtx,pPacket) >=0){
        if (pPacket->stream_index == audio_stream_idx){
            //            解码  mp3   编码格式frame----pcm   frame
            avcodec_decode_audio4(codecContext, frame, &got_frame, pPacket);
            if (got_frame){
                LOGE("解码");
                swr_convert(swrCtx, &out_buffer, 44100 * 2, (const uint8_t **) frame->data, frame->nb_samples);
//                缓冲区的大小
                int size = av_samples_get_buffer_size(NULL, out_channel_nb, frame->nb_samples,
                                                      AV_SAMPLE_FMT_S16, 1);
                *pcm = out_buffer;
                *pcm_size = size;
                break;

            }
        }
    }
    return 0;
}

void releaseFFmpeg() {
    av_free_packet(pPacket);
    av_free(out_buffer);
    av_frame_free(&frame);
    swr_free(&swrCtx);
    avcodec_close(codecContext);
    avformat_close_input(&pFormatCtx);
}