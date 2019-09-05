package io.faceless.speaker

import android.net.Uri
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.NavController
import androidx.navigation.NavHost

class MainActivity : AppCompatActivity(), NavHost, MainFragment.OnFragmentInteractionListener {

    override fun onFragmentInteraction(uri: Uri) {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

    override fun getNavController(): NavController {
        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
    }

    companion object {
        val TAG = MainActivity::class.java.canonicalName as String
    }

    var mRecordBtn: Button? = null



    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

    }
}
