package com.example.battleship.ui

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.TextureView
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import androidx.navigation.fragment.navArgs
import com.example.battleship.R
import com.example.battleship.service.FirebaseService

class ResultFragment: Fragment() {

    private val args: ResultFragmentArgs by navArgs()
    private lateinit var resultTextView: TextView
    private lateinit var mainMenuButton: Button

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_result, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(view: View) {
        resultTextView = view.findViewById(R.id.resultTextView)
        mainMenuButton = view.findViewById(R.id.resultBackToMenuButton)
        mainMenuButton.setOnClickListener {
            val action = ResultFragmentDirections
                .actionResultFragmentToMainMenuFragment()
            FirebaseService.removeGame()
            findNavController().navigate(action)
        }

        when (args.winOrLose) {
            "win" -> {
                resultTextView.text = "You win!"
            }
            "lose" -> {
                resultTextView.text = "You lose!"
            }
        }
        FirebaseService.statsUpload(args.winOrLose)
    }
}