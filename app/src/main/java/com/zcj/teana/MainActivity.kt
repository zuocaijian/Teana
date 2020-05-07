package com.zcj.teana

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.os.PowerManager
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.zcj.teana.ffmpeg.FFmpegCore
import com.zcj.teana.opensles.PlayCore
import com.zcj.teana.ui.VideoActivity
import com.zcj.teana.util.FileUtils
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private val playCore: PlayCore =
        PlayCore()
    private val ffmpegCore: FFmpegCore =
        FFmpegCore()
    private val pcmPath =
        "${App.sContext.getExternalFilesDir(null)}${java.io.File.separator}croatina_44.pcm";
    private val mp3Path =
        "${App.sContext.getExternalFilesDir(null)}${java.io.File.separator}test.mp3";
    private val mp4Path =
        "${App.sContext.getExternalFilesDir(null)}${java.io.File.separator}big_buck_bunny.mp4";

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    fun copyFile(view: View) {
        FileUtils.copyAssetsFile("croatian_44k.pcm", pcmPath)
        FileUtils.copyAssetsFile("test.mp3", mp3Path)
        FileUtils.copyAssetsFile("big_buck_bunny.mp4", mp4Path)
    }

    fun init(view: View) {
        playCore.init()
    }

    fun play(view: View) {
        val pm: PowerManager = getSystemService(Context.POWER_SERVICE) as PowerManager
        val wl: PowerManager.WakeLock =
            pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "Teana:WakeLockTag")
        wl.acquire()
        playCore.play(pcmPath)
        //wl.release()
        /*Thread(Runnable {
            playCore.play(pcmPath)
        }).start()*/
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

    fun videoTest(view: View) {
        startActivity(Intent(this@MainActivity, VideoActivity::class.java))
    }
}
