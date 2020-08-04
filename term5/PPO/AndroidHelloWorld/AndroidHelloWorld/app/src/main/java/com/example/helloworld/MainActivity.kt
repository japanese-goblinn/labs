package com.example.helloworld

import android.annotation.SuppressLint
import android.content.Context
import android.content.pm.PackageManager
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import android.telephony.TelephonyManager
import android.util.Log
import com.google.android.material.snackbar.Snackbar
import android.Manifest.permission
import android.annotation.TargetApi
import android.widget.Toast
import androidx.annotation.RequiresApi
import androidx.core.content.PermissionChecker


class MainActivity : AppCompatActivity() {

    private lateinit var versionTextView: TextView
    private lateinit var deviceIdTextView: TextView

    private val RECORD_REQUEST_CODE = 101

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        versionTextView = findViewById(R.id.versionTextView)
        deviceIdTextView = findViewById(R.id.deviceIdTextView)
        versionTextView.text = getString(R.string.appVersion, BuildConfig.VERSION_NAME)
        Log.d("SSSS", "${BuildConfig.VERSION_CODE}")
        checkPermission()
    }

    @SuppressLint("NewApi")
    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        grantResults: IntArray
    ) {
        when (requestCode) {
            RECORD_REQUEST_CODE -> {
                if (grantResults.isNotEmpty() && grantResults[0]
                        == PackageManager.PERMISSION_GRANTED) {
                    Toast.makeText(
                        this,
                        getString(R.string.permissionAccepted),
                        Toast.LENGTH_LONG
                    )
                    .show()
                    showDeviceId()
                    Log.d("PERMISSION", "Permission was granted")
                } else {
                    if (shouldShowRequestPermissionRationale(permission.READ_PHONE_STATE)) {
                        showExplanationPopUp()
                    }
                    Log.d("PERMISSION", "Has been denied by user")
                }
            }
        }
    }

    private fun showExplanationPopUp() {
        Snackbar.make(
            findViewById(R.id.rootView),
            getString(R.string.acceptPermission),
            Snackbar.LENGTH_INDEFINITE
        )
        .setAction(getString(R.string.acceptPermissionButtonText)) {
            requestPermission()
        }
        .show()
    }

    private fun requestPermission() {
        ActivityCompat.requestPermissions(
            this,
            arrayOf(permission.READ_PHONE_STATE),
            RECORD_REQUEST_CODE
        )
    }

    @SuppressLint("MissingPermission", "NewApi")
    private fun showDeviceId() {
        val telephonyManager = getSystemService(Context.TELEPHONY_SERVICE) as TelephonyManager
        deviceIdTextView.text = getString(R.string.deviceId, telephonyManager.imei)
    }

    private fun checkPermission() {
        if (ContextCompat.checkSelfPermission(this, permission.READ_PHONE_STATE)
                != PackageManager.PERMISSION_GRANTED) {
            requestPermission()
        } else {
            showDeviceId()
            Log.d("PERMISSION", "Already granted")
        }
    }

}
