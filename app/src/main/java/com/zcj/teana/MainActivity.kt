package com.zcj.teana

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import com.zcj.teana.ffmpeg.FFmpegCore
import com.zcj.teana.opensles.PlayCore
import com.zcj.teana.util.FileUtils
import kotlinx.android.synthetic.main.activity_main.*
import kotlin.math.log

class MainActivity : AppCompatActivity() {

    private val playCore: PlayCore =
        PlayCore()
    private val ffmpegCore: FFmpegCore =
        FFmpegCore()
    private val pcmPath =
        "${App.sContext.getExternalFilesDir(null)}${java.io.File.separator}croatina_44.cpm";

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    fun copyFile(view: View) {
        FileUtils.copyAssetsFile("croatian_44k.pcm", pcmPath)
    }

    fun init(view: View) {
        playCore.init()
    }

    fun play(view: View) {
        playCore.play(pcmPath)
    }

    fun pause(view: View) {
        playCore.pause()
    }

    fun stop(view: View) {
        playCore.stop()
    }

    fun release(view: View) {
        playCore.destroy()
    }

    fun getFFmpegVersion(view: View) {
        tv_msg.text =
            "FFmpeg version = ${ffmpegCore.fFmpegVersion}\nlicence = ${ffmpegCore.fFmpegLicense}\nconfiguration = ${ffmpegCore.fFmpegConfiguration}"
    }
}
