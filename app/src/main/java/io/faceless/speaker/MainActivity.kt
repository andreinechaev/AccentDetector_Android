package io.faceless.speaker

import android.Manifest.permission.MODIFY_AUDIO_SETTINGS
import android.Manifest.permission.RECORD_AUDIO
import android.content.Context
import android.content.pm.PackageManager.PERMISSION_GRANTED
import android.media.AudioDeviceInfo
import android.media.AudioDeviceInfo.*
import android.media.AudioManager
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat.checkSelfPermission
import io.faceless.speaker.audio.Repeater
import kotlinx.coroutines.*

class MainActivity : AppCompatActivity() {

    companion object {
        val TAG = MainActivity::class.java.canonicalName as String

        val DEVICE_TYPES = setOf(TYPE_BLUETOOTH_A2DP, TYPE_WIRED_HEADPHONES, TYPE_BUILTIN_EARPIECE)

        val REQUIRED_PERMISSIONS = arrayOf(RECORD_AUDIO, MODIFY_AUDIO_SETTINGS)
    }

    private val mRepeater by lazy { Repeater.instance }

    private val mAudioManager by lazy {
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
        return devices.filter { DEVICE_TYPES.contains(it.type) }
            .maxBy { it.type }!!
    }

    private fun checkPermissions() {
        if (checkSelfPermission(this, RECORD_AUDIO) != PERMISSION_GRANTED
            || checkSelfPermission(this, MODIFY_AUDIO_SETTINGS) != PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this, REQUIRED_PERMISSIONS, 0)
        }
    }


}
