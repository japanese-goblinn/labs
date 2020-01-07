package com.example.battleship.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.R

class WelcomeFragment: Fragment() {

    private lateinit var signUpButton: Button
    private lateinit var signInButton: Button

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val auth = (activity as MainActivity).auth
        auth.currentUser?.let {
            val action = WelcomeFragmentDirections
                .actionWelcomeFragmentToMainMenuFragment()
            findNavController().navigate(action)
        }
        val rootView = inflater.inflate(R.layout.fragment_welcome, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(view: View) {
        signInButton = view.findViewById(R.id.goToSignInButton)
        signInButton.setOnClickListener {
            val action =
                WelcomeFragmentDirections.actionWelcomeFragmentToSignInFragment()
            findNavController().navigate(action)
        }
        signUpButton = view.findViewById(R.id.goToSignUpButton)
        signUpButton.setOnClickListener {
            val action =
                WelcomeFragmentDirections.actionWelcomeFragmentToSignUpFragment()
            findNavController().navigate(action)
        }
    }

}