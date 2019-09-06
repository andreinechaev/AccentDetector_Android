package io.faceless.speaker.audio

import android.media.AudioDeviceInfo.TYPE_BUILTIN_EARPIECE
import android.media.AudioDeviceInfo.TYPE_BUILTIN_MIC
import android.media.AudioFormat
import android.util.Log
import java.util.concurrent.atomic.AtomicBoolean

open class Repeater(
    val rate: Int = 22000, val encoding: Int = AudioFormat.ENCODING_DEFAULT,
    val inputDevice: Int = TYPE_BUILTIN_MIC, val outputDevice: Int = TYPE_BUILTIN_EARPIECE,
    private  val isRunning: AtomicBoolean = AtomicBoolean(false)
) {


    fun init() {
        initEngine(inputDevice, outputDevice)
    }

    fun transmit() {
        if (isRunning.get()) {
            Log.e(TAG, "Cannot transmit, Repeater is still running")
            return
        }
        isRunning.set(true)
        if (!start()) {
            Log.e(TAG, "Could not start Engine")
        } else {
            Log.d(TAG, "Engine Started")
        }
    }

    fun release() {
        if (!stop()) {
            Log.e(TAG, "Could not stop Engine")
        } else {
            Log.d(TAG, "Engine Stopped")
        }
        isRunning.set(false)
    }

    fun isRunning(): Boolean {
        return isRunning.get()
    }

    companion object {
        val TAG = Repeater::class.java.canonicalName as String

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    private external fun initEngine(deviceIn: Int, deviceOut: Int)

    private external fun start(): Boolean

    private external fun stop(): Boolean
}