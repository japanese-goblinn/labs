package com.example.battleship.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.ImageButton
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.R
import com.example.battleship.service.FirebaseService
import com.google.android.material.textfield.TextInputEditText

class SignUpFragment: Fragment() {

    private lateinit var backButton: ImageButton
    private lateinit var emailEditText: TextInputEditText
    private lateinit var passwordEditText: TextInputEditText
    private lateinit var repeatPasswordEditText: TextInputEditText
    private lateinit var signUpButton: Button

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_sign_up, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun passwordsMatch(first: String, second: String) = first == second

    private fun setupLayoutFor(view: View) {

        backButton = view.findViewById(R.id.signUpBackButton)
        backButton.setOnClickListener { findNavController().popBackStack() }

        emailEditText = view.findViewById(R.id.emailSignUpTextInputEditText)
        passwordEditText = view.findViewById(R.id.passwordSignUpTextInputEditText)
        repeatPasswordEditText = view.findViewById(R.id.repeatPasswordSignUpTextInputEditText)

        signUpButton = view.findViewById(R.id.signUpButton)
        signUpButton.setOnClickListener {

            if (emailEditText.text.isNullOrEmpty()) {
                emailEditText.error = "Can not be empty"
                return@setOnClickListener
            }

            if (repeatPasswordEditText.text.isNullOrEmpty()) {
                repeatPasswordEditText.error = "Can not be empty"
                return@setOnClickListener
            }

            val password1 = passwordEditText.text.toString().trim()
            val password2 = repeatPasswordEditText.text.toString().trim()

            if (passwordsMatch(password1, password2)) {
                FirebaseService.auth.createUserWithEmailAndPassword(
                    emailEditText.text.toString().trim(),
                    passwordEditText.text.toString().trim()
                )
                .addOnCompleteListener {
                    if (it.isSuccessful) {
                        val action = SignUpFragmentDirections
                            .actionSignUpFragmentToMainMenuFragment()
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
            } else {
                repeatPasswordEditText.error = "Passwords do not match"
            }
        }
    }
}