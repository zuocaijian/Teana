package com.zcj.teana

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.zcj.teana.pcm.Control
import com.zcj.teana.util.FileUtils

class MainActivity : AppCompatActivity() {

    private val pcmControl: Control = Control()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    fun copyFile(view: View) {
        val filePath =
            "${getExternalFilesDir(null)}${java.io.File.separator}croatina_44.pcm"
        FileUtils.copyAssetsFile("croatian_44k.pcm", filePath)
    }

    fun init(view: View) {
        pcmControl.init()
    }

    fun play(view: View) {
        pcmControl.play("/sdcard/media/test.mp3")
    }

    fun pause(view: View) {
        pcmControl.pause()
    }

    fun stop(view: View) {
        pcmControl.stop()
    }

    fun release(view: View) {
        pcmControl.destroy()
    }
}
