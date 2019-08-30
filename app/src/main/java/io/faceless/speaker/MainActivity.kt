package io.faceless.speaker

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioDeviceInfo
import android.media.AudioDeviceInfo.*
import android.media.AudioManager
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import io.faceless.speaker.audio.Repeater
import kotlinx.coroutines.*

class MainActivity : AppCompatActivity() {

    companion object {
        val TAG = MainActivity::class.java.canonicalName as String

        val DEVICE_TYPES = setOf(TYPE_BLUETOOTH_A2DP, TYPE_WIRED_HEADPHONES, TYPE_BUILTIN_EARPIECE)

        const val BUFFER_SIZE = 32
    }

    private val mRepeater by lazy { Repeater(bufferSize = BUFFER_SIZE) }

    private val mAudioManager: AudioManager by lazy {
        getSystemService(Context.AUDIO_SERVICE) as AudioManager
    }

    private var mRecordBtn: Button? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        checkPermissions()

        mRepeater.track.preferredDevice = audioDeviceInfo()

        mRecordBtn = findViewById(R.id.record_play_pause)

        var launch: Job? = null
        mRecordBtn?.setOnClickListener {
            if (launch != null && launch!!.isActive) {
                launch!!.cancel()
                mRepeater.release()
                mRecordBtn!!.setBackgroundResource(R.drawable.record_button_start)
            } else {
                mRecordBtn!!.setBackgroundResource(R.drawable.record_button_stop)
                launch = GlobalScope.launch(Dispatchers.Default + CoroutineName("Transmitter")) {
                    mRepeater.prepare()
                    val buffer = ByteArray(mRepeater.bufferSize)
                    while (true) {
                        mRepeater.transmit(buffer)
                    }
                }
            }
        }
    }

    private fun audioDeviceInfo(): AudioDeviceInfo? {
        val devices = mAudioManager.getDevices(AudioManager.GET_DEVICES_OUTPUTS)
        val sorted = devices.filter { DEVICE_TYPES.contains(it.type) }
            .sortedByDescending { it.type }
        val deviceInfo = sorted.first()
        return deviceInfo
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
            val permissions =
                arrayOf(Manifest.permission.RECORD_AUDIO, Manifest.permission.MODIFY_AUDIO_SETTINGS)
            ActivityCompat.requestPermissions(this, permissions, 0)
        }
    }


}
