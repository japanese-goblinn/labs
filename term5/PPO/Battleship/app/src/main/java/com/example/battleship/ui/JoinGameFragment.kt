package com.example.battleship.ui

import android.app.AlertDialog
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.R
import com.example.battleship.service.FirebaseService
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener
import org.w3c.dom.Text

class JoinGameFragment: Fragment() {

    private lateinit var backButton: ImageButton
    private lateinit var gameCodeEditText: EditText
    private lateinit var joinButton: Button
    private lateinit var dialog: androidx.appcompat.app.AlertDialog

    private var gameStartListener: ValueEventListener? = null
    private var isNavigationDismiss = false

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_join_game, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    override fun onDestroyView() {
        super.onDestroyView()
        gameStartListener?.let {
            FirebaseService
                .gamesRef
                .child( gameCodeEditText.text.toString().toUpperCase())
                .child("started")
                .removeEventListener(it)
        }
    }

    private fun setupLayoutFor(view: View) {
        backButton = view.findViewById(R.id.joinGameBackButton)
        backButton.setOnClickListener { findNavController().popBackStack() }

        gameCodeEditText = view.findViewById(R.id.gameCodeTextInputEditText)
        joinButton = view.findViewById(R.id.joinButton)
        joinButton.setOnClickListener {
            FirebaseService.joinGame(
                gameCodeEditText.text.toString().toUpperCase(),
                FirebaseService.auth.currentUser
            ) { isGameExists ->
                if (isGameExists) {
                    val builder = androidx.appcompat.app.AlertDialog.Builder(
                        requireContext(), R.style.dialog
                    )
                    val view = layoutInflater.inflate(R.layout.dialog_wait_host, null)
                    builder.setView(view)
                    dialog = builder.create()
                    dialog.setOnDismissListener {
                        if (!isNavigationDismiss) {
                            FirebaseService.leaveGame(
                                gameCodeEditText.text.toString().toUpperCase(),
                                FirebaseService.auth.currentUser
                            )
                        }
                    }
                    dialog.show()
                    gameStartListener = object: ValueEventListener {
                        override fun onDataChange(dataSnapshot: DataSnapshot) {
                            if (dataSnapshot.value == true) {
                                isNavigationDismiss = true
                                dialog.dismiss()
                                (activity as MainActivity).snowfallView.stopFalling()
                                val action = JoinGameFragmentDirections
                                    .actionJoinGameFragmentToBattlefieldFragment()
                                if (findNavController().currentDestination?.id == R.id.joinGameFragment) {
                                    findNavController().navigate(action)
                                }
                            }
                        }

                        override fun onCancelled(databaseError: DatabaseError) {
                            Log.w("SSS", "GAME CONNECTION CANCELED", databaseError.toException())
                        }
                    }

                    FirebaseService
                        .gamesRef
                        .child( gameCodeEditText.text.toString().toUpperCase())
                        .child("started")
                        .addValueEventListener(gameStartListener!!)
                } else {
                    Toast
                        .makeText(
                            requireContext(),
                            "This game is not exist",
                            Toast.LENGTH_LONG
                        )
                        .show()
                }
            }
        }
    }
}