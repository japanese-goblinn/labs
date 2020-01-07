package com.example.battleship.service

import android.util.Log
import android.view.View
import com.example.battleship.R
import com.google.firebase.FirebaseApp
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.FirebaseUser
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import java.lang.Exception

class FirebaseService {
    companion object {
        val auth = FirebaseAuth.getInstance()
        val gamesRef = FirebaseDatabase.getInstance().getReference("games")

        fun createGame(code: String, user: FirebaseUser?) {
            user?.let {
                gamesRef
                    .child(code)
                    .child("players")
                    .child(it.uid)
                    .setValue(true)
                changeGame(code, false)
            }
        }

        fun joinGame(code: String, user: FirebaseUser?, isGameExists: (Boolean) -> Unit) {
            user?.let {
                gamesRef
                    .child(code)
                    .addListenerForSingleValueEvent(object : ValueEventListener {
                        override fun onDataChange(dataSnapshot: DataSnapshot) {
                            if (dataSnapshot.value == null) {
                                isGameExists(false)
                            } else {
                                isGameExists(true)
                                gamesRef
                                    .child(code)
                                    .child("players")
                                    .child(it.uid)
                                    .setValue(true)
                            }
                        }

                        override fun onCancelled(databaseError: DatabaseError) {
                            Log.w("SSS", "JOIN GAME FAILED", databaseError.toException())
                        }
                    })
            }
        }

        fun leaveGame(code: String, user: FirebaseUser?) {
            user?.let {
                gamesRef
                    .child(code)
                    .child("players")
                    .child(it.uid)
                    .removeValue()
            }
        }

        fun removeGameWith(code: String) {
            gamesRef
                .child(code)
                .removeValue()
        }

        fun changeGame(code: String, state: Boolean) {
            gamesRef
                .child(code)
                .child("started")
                .setValue(state)
        }

    }
}