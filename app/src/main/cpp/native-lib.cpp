#include <jni.h>
#include <string.h>
#include <android/log.h>
extern "C" {
#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
};
#include <unistd.h>
#include "include/FFmpegMusic.h"

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG,"YJ_FFMPGE_DEMO------>>",FORMAT,##__VA_ARGS__);
SLObjectItf engineObject = NULL;//用SLObjectItf声明引擎接口对象
SLEngineItf engineEngine = NULL;//声明具体的引擎对象

SLObjectItf outputMixObject = NULL;//用SLObjectItf创建混音器接口对象
SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;//具体的混音器对象实例
SLEnvironmentalReverbSettings settings = SL_I3DL2_ENVIRONMENT_PRESET_DEFAULT;//默认情况

SLObjectItf audioplayer=NULL;//用SLObjectItf声明播放器接口对象
SLPlayItf  slPlayItf=NULL;//播放器接口
SLAndroidSimpleBufferQueueItf  slBufferQueueItf=NULL;//缓冲区队列接口

size_t buffersize =0;
void *buffer;
//将pcm数据添加到缓冲区中
void getQueueCallBack(SLAndroidSimpleBufferQueueItf  slBufferQueueItf, void* context){

    buffersize=0;

    getPcm(&buffer,&buffersize);
    if(buffer!=NULL&&buffersize!=0){
        //将得到的数据加入到队列中
        (*slBufferQueueItf)->Enqueue(slBufferQueueItf,buffer,buffersize);
    }
}

void createEngine(){
    slCreateEngine(&engineObject,0,NULL,0,NULL,NULL);//创建引擎
    (*engineObject)->Realize(engineObject,SL_BOOLEAN_FALSE);//实现engineObject接口对象
    (*engineObject)->GetInterface(engineObject,SL_IID_ENGINE,&engineEngine);//通过引擎调用接口初始化SLEngineItf
}

//创建混音器
void createMixVolume(){
    (*engineEngine)->CreateOutputMix(engineEngine,&outputMixObject,0,0,0);//用引擎对象创建混音器接口对象
    (*outputMixObject)->Realize(outputMixObject,SL_BOOLEAN_FALSE);//实现混音器接口对象
    SLresult   sLresult = (*outputMixObject)->GetInterface(outputMixObject,SL_IID_ENVIRONMENTALREVERB,&outputMixEnvironmentalReverb);//利用混音器实例对象接口初始化具体的混音器对象
    //设置
    if (SL_RESULT_SUCCESS == sLresult) {
        (*outputMixEnvironmentalReverb)->
                SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb, &settings);
    }
}

//创建播放器
void createPlayer() {
    //初始化ffmpeg
    int rate;
    int channels;
    createFFmpeg(&rate, &channels);
    LOGE("RATE %d", rate);
    LOGE("channels %d", channels);
    /*
     * typedef struct SLDataLocator_AndroidBufferQueue_ {
    SLuint32    locatorType;//缓冲区队列类型
    SLuint32    numBuffers;//buffer位数
} */


    SLDataLocator_AndroidBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,2};
    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM,(unsigned int)channels, (unsigned int)rate*1000
            ,SL_PCMSAMPLEFORMAT_FIXED_16
            ,SL_PCMSAMPLEFORMAT_FIXED_16
            ,SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,SL_BYTEORDER_LITTLEENDIAN};

    SLDataSource dataSource = {&android_queue,&pcm};


    SLDataLocator_OutputMix slDataLocator_outputMix={SL_DATALOCATOR_OUTPUTMIX,outputMixObject};


    SLDataSink slDataSink = {&slDataLocator_outputMix,NULL};


    const SLInterfaceID ids[3]={SL_IID_BUFFERQUEUE,SL_IID_EFFECTSEND,SL_IID_VOLUME};
    const SLboolean req[3]={SL_BOOLEAN_FALSE,SL_BOOLEAN_FALSE,SL_BOOLEAN_FALSE};
    LOGE("执行到此处")
    (*engineEngine)->CreateAudioPlayer(engineEngine,&audioplayer,&dataSource,&slDataSink,3,ids,req);
    (*audioplayer)->Realize(audioplayer,SL_BOOLEAN_FALSE);
    LOGE("执行到此处2")
    (*audioplayer)->GetInterface(audioplayer,SL_IID_PLAY,&slPlayItf);//初始化播放器
//注册缓冲区,通过缓冲区里面 的数据进行播放
    (*audioplayer)->GetInterface(audioplayer,SL_IID_BUFFERQUEUE,&slBufferQueueItf);
//设置回调接口
    (*slBufferQueueItf)->RegisterCallback(slBufferQueueItf,getQueueCallBack,NULL);
//播放
    (*slPlayItf)->SetPlayState(slPlayItf,SL_PLAYSTATE_PLAYING);

//开始播放
    getQueueCallBack(slBufferQueueItf,NULL);
}

//释放资源
void realseResource(){
    if(audioplayer!=NULL){
        (*audioplayer)->Destroy(audioplayer);
        audioplayer=NULL;
        slBufferQueueItf=NULL;
        slPlayItf=NULL;
    }
    if(outputMixObject!=NULL){
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject=NULL;
        outputMixEnvironmentalReverb=NULL;
    }
    if(engineObject!=NULL){
        (*engineObject)->Destroy(engineObject);
        engineObject=NULL;
        engineEngine=NULL;
    }
    releaseFFmpeg();
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_tuyasmart_media_FFmpegMedia_play(JNIEnv *env, jobject instance) {
    createEngine();
    createMixVolume();
    createPlayer();

}

JNIEXPORT void JNICALL
Java_com_tuyasmart_media_FFmpegMedia_stop(JNIEnv *env, jobject instance) {
    realseResource();
}

#ifdef __cplusplus
}
#endif



