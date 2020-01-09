package com.example.battleship.service

import android.util.Log
import android.view.View
import com.example.battleship.R
import com.example.battleship.models.Ship
import com.google.firebase.FirebaseApp
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.FirebaseUser
import com.google.firebase.database.*
import java.lang.Exception

class FirebaseService {
    companion object {

        var gameCode = ""

        var enemyUid = ""

        val auth = FirebaseAuth.getInstance()

        val gamesRef = FirebaseDatabase.getInstance().getReference("games")

        val currentGameRef: DatabaseReference
            get() = gamesRef.child(gameCode)

        fun upload(ships: List<Ship>, user: FirebaseUser?) {
            user?.let { user ->
                ships.forEach { ship ->
                    val key = currentGameRef
                        .child("players")
                        .child(user.uid)
                        .child("ships")
                        .push()
                        .key!!
                    currentGameRef
                        .child("players")
                        .child(user.uid)
                        .child("ships")
                        .child(key)
                        .setValue(ship)
                }
            }
        }

        fun createGame(code: String, user: FirebaseUser?) {
            user?.let {
                gameCode = code
                currentGameRef
                    .child("players")
                    .child(it.uid)
                    .setValue(true)
                changeGame(false)
            }
        }

        fun removeGame() = currentGameRef.removeValue()

        fun joinGame(code: String, user: FirebaseUser?, isGameExists: (Boolean) -> Unit) {
            if (code.isNullOrEmpty()) {
                return
            }
            user?.let {
                gamesRef
                    .child(code)
                    .addListenerForSingleValueEvent(object : ValueEventListener {
                        override fun onDataChange(dataSnapshot: DataSnapshot) {
                            if (dataSnapshot.value == null) {
                                isGameExists(false)
                            } else {
                                isGameExists(true)
                                gameCode = code
                                currentGameRef
                                    .child("players")
                                    .child(it.uid)
                                    .setValue(true)
                            }
                        }

                        override fun onCancelled(databaseError: DatabaseError) {
                            Log.d("SSS", "JOIN GAME FAILED", databaseError.toException())
                        }
                    })
            }
        }

        fun leaveGame( user: FirebaseUser?) {
            user?.let {
                currentGameRef
                    .child("players")
                    .child(it.uid)
                    .removeValue()
            }
        }

        fun changeGame(state: Boolean) {
            currentGameRef
                .child("started")
                .setValue(state)
        }

        fun continueGame(user: FirebaseUser?) {
            user?.let {
                currentGameRef.child("ready").child(it.uid).setValue(true)
            }
        }

    }
}