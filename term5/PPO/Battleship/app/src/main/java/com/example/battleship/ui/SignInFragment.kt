package com.example.battleship.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import android.widget.ImageButton
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.R
import com.example.battleship.service.FirebaseService

class SignInFragment: Fragment() {

    private lateinit var backButton: ImageButton
    private lateinit var emailEditText: EditText
    private lateinit var passwordEditText: EditText
    private lateinit var signInButton: Button

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_sign_in, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(view: View) {

        backButton = view.findViewById(R.id.signInBackButton)
        backButton.setOnClickListener { findNavController().popBackStack() }

        emailEditText = view.findViewById(R.id.emailSignInTextInputEditText)
        passwordEditText = view.findViewById(R.id.passwordSignInTextInputEditText)
        signInButton = view.findViewById(R.id.signInButton)
        signInButton.setOnClickListener {

            if (emailEditText.text.isNullOrEmpty()) {
                emailEditText.error = "Can not be empty"
                return@setOnClickListener
            }

            if (passwordEditText.text.isNullOrEmpty()) {
                passwordEditText.error = "Can not be empty"
                return@setOnClickListener
            }

            FirebaseService.auth.signInWithEmailAndPassword(
                emailEditText.text.toString().trim(),
                passwordEditText.text.toString().trim()
            )
            .addOnCompleteListener {
                if (it.isSuccessful) {
                    val action = SignInFragmentDirections
                        .actionSignInFragmentToMainMenuFragment()
                    findNavController().navigate(action)
                } else {
                    Toast
                        .makeText(
                            requireContext(),
                            it.exception?.localizedMessage,
                            Toast.LENGTH_LONG
                        )
                        .show()
                }
            }
        }
    }

}