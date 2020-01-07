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

class MainMenuFragment: Fragment() {

    private lateinit var signOutButton: Button
    private lateinit var emailTextView: TextView

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_main_menu, container, false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(view: View) {

        val auth = (activity as MainActivity).auth
        val user = auth.currentUser!!

        Toast
            .makeText(
                requireContext(),
                "Welcome to the battleship, sailor",
                Toast.LENGTH_LONG
            )
            .show()

        emailTextView = view.findViewById(R.id.emailTextView)
        emailTextView.text = user.email.toString()

        signOutButton = view.findViewById(R.id.signOutButton)
        signOutButton.setOnClickListener {
            auth.signOut()
            val action = MainMenuFragmentDirections
                .actionMainMenuFragmentToWelcomeFragment()
            findNavController().navigate(action)
        }
    }
}