package com.example.battleship.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.R
import com.example.battleship.service.FirebaseService

class MainMenuFragment: Fragment() {

    private lateinit var signOutButton: Button
    private lateinit var emailTextView: TextView
    private lateinit var createGameButton: Button
    private lateinit var joinGameButton: Button
    private lateinit var statisticsButton: Button

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_main_menu, container, false)
        setupLayoutFor(rootView)
        return rootView
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Toast
            .makeText(
                requireContext(),
                "Welcome to the battleship, sailor",
                Toast.LENGTH_LONG
            )
            .show()
    }

    private fun setupLayoutFor(view: View) {

        emailTextView = view.findViewById(R.id.emailTextView)
        emailTextView.text = FirebaseService.auth.currentUser?.email.toString()

        signOutButton = view.findViewById(R.id.signOutButton)
        signOutButton.setOnClickListener {
            FirebaseService.auth.signOut()
            val action = MainMenuFragmentDirections
                .actionMainMenuFragmentToWelcomeFragment()
            findNavController().navigate(action)
        }

        createGameButton = view.findViewById(R.id.createGameButton)
        createGameButton.setOnClickListener {
            val action = MainMenuFragmentDirections
                .actionMainMenuFragmentToCreateGameFragment()
            findNavController().navigate(action)
        }

        joinGameButton = view.findViewById(R.id.joinGameButton)
        joinGameButton.setOnClickListener {
            val action = MainMenuFragmentDirections
                .actionMainMenuFragmentToJoinGameFragment()
            findNavController().navigate(action)
        }

        statisticsButton = view.findViewById(R.id.statisticsButton)
        statisticsButton.setOnClickListener {
            val action = MainMenuFragmentDirections
                .actionMainMenuFragmentToStatisticsFragment()
            findNavController().navigate(action)
        }
    }
}