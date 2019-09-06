package io.faceless.speaker

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.media.AudioDeviceInfo
import android.media.AudioManager
import android.net.Uri
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.ScrollView
import android.widget.TextView
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import io.faceless.speaker.audio.Repeater
import kotlinx.android.synthetic.main.fragment_main.*

class MainFragment : Fragment() {
    private var listener: OnFragmentInteractionListener? = null

    private var mRecordBtn: Button? = null

    private var mScrollView: ScrollView? = null

    private var mTextView: TextView? = null

    private var mNextBtn: Button? = null

    private var mPreviousBtn: Button? = null

    private var counter: Int = 0

    private val mAudioManager by lazy {
        context?.getSystemService(Context.AUDIO_SERVICE) as AudioManager
    }

    private val mRepeater by lazy {
        val out = audioDeviceInfo()!!.id
        Repeater(outputDevice = out)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mRepeater.init()
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_main, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        checkPermissions()
        mRecordBtn = record_play_pause
        record_play_pause?.setOnClickListener {
            if (mRepeater.isRunning()) {
                Log.d(MainActivity.TAG, "Stopping")
                mRepeater.release()
            } else {
                Log.d(MainActivity.TAG, "Starting")
                mRepeater.transmit()
            }
        }

        mTextView = twister_text_view
        mTextView?.text = Data.twisters[counter]

        mNextBtn = next_twister_btn
        mNextBtn?.setOnClickListener {
            mPreviousBtn?.isEnabled = true
            counter = (counter + 1) % Data.twisters.size
            mTextView?.text = Data.twisters[counter]
        }

        mPreviousBtn = prev_twister_btn
        mPreviousBtn?.setOnClickListener {
            if (counter > 0) {
                counter = (counter - 1) % Data.twisters.size
            } else {
                counter = 0
                mPreviousBtn?.isEnabled = false
            }

            mTextView?.text = Data.twisters[counter]
        }
    }

    override fun onPause() {
        super.onPause()
        if (mRepeater.isRunning()) {
            mRepeater.release()
        }
    }

    override fun onAttach(context: Context) {
        super.onAttach(context)
        if (context is OnFragmentInteractionListener) {
            listener = context
        } else {
            throw RuntimeException("$context must implement OnFragmentInteractionListener")
        }
    }

    override fun onDetach() {
        super.onDetach()
        listener = null
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     *
     *
     * See the Android Training lesson [Communicating with Other Fragments]
     * (http://developer.android.com/training/basics/fragments/communicating.html)
     * for more information.
     */
    interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        fun onFragmentInteraction(uri: Uri)
    }

    companion object {

        val TAG = MainFragment::class.java.canonicalName as String

        val DEVICE_TYPES = setOf(
            AudioDeviceInfo.TYPE_BLUETOOTH_A2DP,
            AudioDeviceInfo.TYPE_WIRED_HEADPHONES,
            AudioDeviceInfo.TYPE_BUILTIN_EARPIECE
        )

        val REQUIRED_PERMISSIONS = arrayOf(
            Manifest.permission.RECORD_AUDIO,
            Manifest.permission.MODIFY_AUDIO_SETTINGS
        )
    }

    private fun checkPermissions() {
        if (ContextCompat.checkSelfPermission(
                this.context!!,
                Manifest.permission.RECORD_AUDIO
            ) != PackageManager.PERMISSION_GRANTED
            || ContextCompat.checkSelfPermission(
                this.context!!,
                Manifest.permission.MODIFY_AUDIO_SETTINGS
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this.activity!!, REQUIRED_PERMISSIONS, 0)
        }
    }

    private fun audioDeviceInfo(): AudioDeviceInfo? {
        val devices = mAudioManager.getDevices(AudioManager.GET_DEVICES_OUTPUTS)
        return devices.filter { DEVICE_TYPES.contains(it.type) }
            .maxBy { it.type }!!
    }
}

private fun AudioManager.requestAudioFocus(onAudioFocusChangeListener: AudioManager.OnAudioFocusChangeListener) {
}
