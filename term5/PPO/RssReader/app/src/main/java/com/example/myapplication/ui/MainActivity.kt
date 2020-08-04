package com.example.myapplication.ui

import android.content.IntentFilter
import android.net.ConnectivityManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.navigation.Navigation
import androidx.navigation.findNavController
import androidx.navigation.ui.setupActionBarWithNavController
import com.example.myapplication.R
import com.example.myapplication.network.NetworkStateReceiver

class MainActivity : AppCompatActivity() {

    private lateinit var receiver: NetworkStateReceiver

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        receiver = NetworkStateReceiver()
        registerReceiver(receiver,
            IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION))

        val navController = findNavController(R.id.navHostFragment)
        setupActionBarWithNavController(navController)
    }

    override fun onDestroy() {
        super.onDestroy()
        unregisterReceiver(receiver)
    }

    override fun onSupportNavigateUp(): Boolean = Navigation.findNavController(
        this,
        R.id.navHostFragment
    ).navigateUp()
}
