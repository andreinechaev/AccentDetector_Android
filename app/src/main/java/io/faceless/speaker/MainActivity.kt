package io.faceless.speaker

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioDeviceInfo
import android.media.AudioManager
import android.os.Bundle
import android.util.Log
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import io.faceless.speaker.audio.Repeater
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private val mAudioManager by lazy {
        getSystemService(Context.AUDIO_SERVICE) as AudioManager
    }

    private val mRepeater by lazy {
        val out = audioDeviceInfo()!!.id
        Repeater(outputDevice = out)
    }

    companion object {
        val TAG = MainActivity::class.java.canonicalName as String

        val DEVICE_TYPES = setOf(
            AudioDeviceInfo.TYPE_BLUETOOTH_A2DP,
            AudioDeviceInfo.TYPE_WIRED_HEADPHONES,
            AudioDeviceInfo.TYPE_BUILTIN_EARPIECE
        )

        val REQUIRED_PERMISSIONS = arrayOf(
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.MODIFY_AUDIO_SETTINGS
        )

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    var mRecordBtn: Button? = null



    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        checkPermissions()

        mRecordBtn = findViewById(R.id.record_play_pause)
        mRecordBtn?.setOnClickListener {
            if (mRepeater.isRunning()) {
                Log.d(TAG, "Stopping")
                mRepeater.release()
            } else {
                Log.d(TAG, "Starting")
                mRepeater.transmit()
            }
        }
        // Example of a call to a native method
        sample_text.text = "Welcome"
    }

    private fun checkPermissions() {
        if (ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.RECORD_AUDIO
            ) != PackageManager.PERMISSION_GRANTED
            || ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.MODIFY_AUDIO_SETTINGS
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this, REQUIRED_PERMISSIONS, 0)
        }
    }

    private fun audioDeviceInfo(): AudioDeviceInfo? {
        val devices = mAudioManager.getDevices(AudioManager.GET_DEVICES_OUTPUTS)
        return devices.filter { DEVICE_TYPES.contains(it.type) }
            .maxBy { it.type }!!
    }
}
