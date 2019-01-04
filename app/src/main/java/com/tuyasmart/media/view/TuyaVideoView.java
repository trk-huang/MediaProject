package com.tuyasmart.media.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.tuyasmart.media.FFmpegMedia;

/**
 * huangdaju
 * 2018/12/29
 **/

public class TuyaVideoView extends SurfaceView {
    private FFmpegMedia mediaPlayer;
    private Surface mSurface;

    public TuyaVideoView(Context context) {
        super(context);
        init();
    }

    public TuyaVideoView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public TuyaVideoView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        mSurface = getHolder().getSurface();
        mediaPlayer = new FFmpegMedia();
    }

    public void startPlay(final String videoPath) {
        mediaPlayer.play();
    }

    public void stop(View view){
        mediaPlayer.stop();
    }
}
