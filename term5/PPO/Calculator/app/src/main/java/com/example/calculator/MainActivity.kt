package com.example.calculator

import android.graphics.PorterDuff
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import android.widget.Button
import androidx.core.app.ComponentActivity
import androidx.core.app.ComponentActivity.ExtraData
import androidx.core.content.ContextCompat
import androidx.core.content.ContextCompat.getSystemService
import org.mariuszgromada.math.mxparser.Expression



class MainActivity : AppCompatActivity() {

    private lateinit var button0: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        button0 = findViewById(R.id.button0)

        button0.setOnClickListener {
            Log.d("SSSS", "0 clicked")
        }
        buttonEffect(button0)
        val e = Expression("2^2+3.3")
        val ssss = e.calculate()
        Log.d("SSSS", "$ssss")
    }

    private fun buttonEffect(button: Button) {
        button.setOnTouchListener { v, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    v.background.setColorFilter(ContextCompat.getColor(applicationContext, R.color.secondActionColor), PorterDuff.Mode.SRC_ATOP)
                    v.invalidate()
                }
                MotionEvent.ACTION_UP -> {
                    v.background.clearColorFilter()
                    v.invalidate()
                }
            }
            false
        }
    }
}
