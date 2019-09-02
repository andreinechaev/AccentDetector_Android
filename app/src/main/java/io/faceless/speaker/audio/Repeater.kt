package io.faceless.speaker.audio

import android.media.AudioAttributes
import android.media.AudioAttributes.CONTENT_TYPE_SPEECH
import android.media.AudioAttributes.USAGE_MEDIA
import android.media.AudioFormat
import android.media.AudioFormat.CHANNEL_IN_MONO
import android.media.AudioFormat.CHANNEL_OUT_MONO
import android.media.AudioRecord
import android.media.AudioTrack
import android.util.Log

open class Repeater(
    val rate: Int = 22000, val encoding: Int = AudioFormat.ENCODING_DEFAULT,
    val chIn: Int = CHANNEL_IN_MONO, val chOut: Int = CHANNEL_OUT_MONO,
    val contentType: Int = CONTENT_TYPE_SPEECH, val usage: Int = USAGE_MEDIA,
    var bufferSize: Int = 64
) {

    val record: AudioRecord by lazy {
        val formatIn = format(chIn)
        AudioRecord.Builder()
            .setAudioFormat(formatIn)
            .setBufferSizeInBytes(bufferSize)
            .build()
    }

    val track: AudioTrack by lazy {
        val formatOut = format(chOut)
        val attributes = attributes()

        AudioTrack.Builder()
            .setAudioFormat(formatOut)
            .setAudioAttributes(attributes)
            .setTransferMode(AudioTrack.MODE_STREAM)
            .build()
    }

    fun prepare() {
        record.startRecording()
        Log.d(TAG, "start recording")
        track.play()
        Log.d(TAG, "start playing")
    }

    fun transmit(buffer: ByteArray) {
        record.read(buffer, 0, bufferSize)
        track.write(buffer, 0, buffer.size)
    }

    fun release() {
        record.stop()
        track.flush()
        track.stop()
    }

    fun attributes(): AudioAttributes {
        return AudioAttributes.Builder()
            .setContentType(contentType)
            .setUsage(usage)
            .build()
    }

    fun format(channel: Int): AudioFormat {
        return AudioFormat.Builder()
            .setSampleRate(rate)
            .setChannelMask(channel)
            .setEncoding(encoding)
            .build()
    }

    fun minBufSize(): Int {
        return AudioRecord.getMinBufferSize(
            rate,
            chIn,
            encoding
        )
    }

    companion object {
        val TAG = Repeater::class.java.canonicalName as String
        val instance = Repeater()
    }
}