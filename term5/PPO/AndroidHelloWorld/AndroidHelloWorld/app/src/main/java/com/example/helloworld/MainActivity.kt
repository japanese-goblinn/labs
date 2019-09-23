package com.example.helloworld

import android.Manifest
import android.annotation.SuppressLint
import android.content.Context
import android.content.pm.PackageManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import android.telephony.TelephonyManager




class MainActivity : AppCompatActivity() {

    private lateinit var versionTextView: TextView
    private lateinit var deviceIdTextView: TextView

    private val RECORD_REQUEST_CODE = 101

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        versionTextView = findViewById(R.id.versionTextView)
        deviceIdTextView = findViewById(R.id.deviceIdTextView)
        checkPermission()
        versionTextView.text = getString(R.string.appVersion, 0)
    }

    @SuppressLint("MissingPermission")
    override fun onRequestPermissionsResult(requestCode: Int,
                                            permissions: Array<String>, grantResults: IntArray) {
        when (requestCode) {
            RECORD_REQUEST_CODE -> {
                if (grantResults.isEmpty() || grantResults[0] != PackageManager.PERMISSION_GRANTED) {
                    print("Permission has been denied by  user")
                } else {
                    val telephonyManager =
                        getSystemService(Context.TELEPHONY_SERVICE) as TelephonyManager
                    val deviceID = telephonyManager.deviceId
                    deviceIdTextView.text = getString(R.string.deviceID, deviceID)

                }
            }
        }
    }

    private fun checkPermission() {
        val permission = ContextCompat.checkSelfPermission(this,
            Manifest.permission.READ_PHONE_STATE)
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_PHONE_STATE)
            != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.READ_PHONE_STATE),
                RECORD_REQUEST_CODE
            )
        }
    }

}
