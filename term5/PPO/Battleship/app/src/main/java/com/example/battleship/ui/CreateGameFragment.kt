package com.example.battleship.ui

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

class CreateGameFragment: Fragment() {

    private lateinit var backButton: ImageButton
    private lateinit var gameCodeTextView: TextView
    private lateinit var startGameButton: Button
    private lateinit var progressBar: ProgressBar

    private var playersListener: ValueEventListener? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_create_game, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    override fun onDestroyView() {
        super.onDestroyView()
        playersListener?.let {
            FirebaseService
                .currentGameRef
                .child("players")
                .removeEventListener(it)
        }
    }

    private fun setupLayoutFor(view: View) {

        progressBar = view.findViewById(R.id.progressBar)

        gameCodeTextView = view.findViewById(R.id.gameCodeTextView)
        gameCodeTextView.text = FirebaseService.auth
            .currentUser
            ?.uid
            ?.substring(0..5)
            ?.toUpperCase()

        backButton = view.findViewById(R.id.createGameBackButton)
        backButton.setOnClickListener {
            FirebaseService.removeGame()
            findNavController().popBackStack()
        }

        startGameButton = view.findViewById(R.id.continueGameButton)
        startGameButton.setOnClickListener {
            FirebaseService.changeGame(true)
            val action = CreateGameFragmentDirections
                .actionCreateGameFragmentToShipArrangementFragment()
            findNavController().navigate(action)
        }

        FirebaseService.createGame(
            gameCodeTextView.text.toString(),
            FirebaseService.auth.currentUser
        )

        playersListener = object: ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                if (dataSnapshot.childrenCount.toInt() == 2) {
                    for (player in dataSnapshot.children) {
                        val value = player.key.toString()
                        if (value != FirebaseService.auth.currentUser!!.uid) {
                            FirebaseService.enemyUid = value
                        }
                    }
                    progressBar.visibility = View.GONE
                    startGameButton.isEnabled = true
                    startGameButton.setBackgroundColor(resources.getColor(R.color.colorAccent))
                    startGameButton.setTextColor(resources.getColor(R.color.colorTextPrimary))

                } else {
                    progressBar.visibility = View.VISIBLE
                    startGameButton.isEnabled = false
                    startGameButton.setBackgroundColor(resources.getColor(R.color.colorPrimary))
                    startGameButton.setTextColor(resources.getColor(R.color.colorElement))
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                Log.w("SSS", "GAME CONNECTION CANCELED", databaseError.toException())
            }
        }

        FirebaseService
            .currentGameRef
            .child("players")
            .addValueEventListener(playersListener!!)
    }
}