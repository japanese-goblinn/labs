package com.example.myapplication.network

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.net.ConnectivityManager
import android.widget.Toast
import androidx.core.content.ContextCompat.getSystemService

class NetworkStateReceiver: BroadcastReceiver() {

    override fun onReceive(context: Context?, intent: Intent?) {
        if (isConnectedOrConnecting(context)) {
            Toast
                .makeText(context, "Internet is connected", Toast.LENGTH_LONG)
                .show()
        } else {
            Toast
                .makeText(context, "Internet is not available", Toast.LENGTH_LONG)
                .show()
        }
    }

    private fun isConnectedOrConnecting(context: Context?): Boolean {
        val manager = context
            ?.getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager
        val networkInfo = manager.activeNetworkInfo
        return networkInfo?.let { it.isConnectedOrConnecting } ?: false
    }
}