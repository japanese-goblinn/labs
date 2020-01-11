package com.example.battleship.service

import android.util.Log
import android.view.View
import com.example.battleship.R
import com.example.battleship.models.CellType
import com.example.battleship.models.Hit
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
        val statsRef = FirebaseDatabase.getInstance().getReference("stats")
        val currentGameRef: DatabaseReference
            get() = gamesRef.child(gameCode)

        fun statsUpload(winOrLose: String) {
            auth.currentUser?.let { user ->
                when (winOrLose) {
                    "win" -> {
                        statsRef
                            .child(user.uid)
                            .child("wins")
                            .addListenerForSingleValueEvent(object : ValueEventListener {
                                override fun onDataChange(dataSnapshot: DataSnapshot) {
                                    if (dataSnapshot.value == null) {
                                        statsRef.child(user.uid).child("wins").setValue(1)
                                    } else {
                                        val newValue = dataSnapshot.getValue(Int::class.java)!! + 1
                                        statsRef.child(user.uid).child("wins")
                                            .setValue(newValue)
                                    }
                                }

                                override fun onCancelled(databaseError: DatabaseError) {
                                    Log.d("SSS", "STATS READ FAILED", databaseError.toException())
                                }
                            })
                    }
                    "lose" -> {
                        statsRef
                            .child(user.uid)
                            .child("loses")
                            .addListenerForSingleValueEvent(object : ValueEventListener {
                                override fun onDataChange(dataSnapshot: DataSnapshot) {
                                    if (dataSnapshot.value == null) {
                                        statsRef.child(user.uid).child("loses").setValue(1)
                                    } else {
                                        val newValue = dataSnapshot.getValue(Int::class.java)!! + 1
                                        statsRef.child(user.uid).child("loses")
                                            .setValue(newValue)
                                    }
                                }

                                override fun onCancelled(databaseError: DatabaseError) {
                                    Log.d("SSS", "STATS READ FAILED", databaseError.toException())
                                }
                            })
                    }
                }
            }
        }

        fun statsUpload(type: CellType) {
            auth.currentUser?.let { user ->
                when (type) {
                    CellType.MISS -> {
                        statsRef
                            .child(user.uid)
                            .child("misses")
                            .addListenerForSingleValueEvent(object : ValueEventListener {
                                override fun onDataChange(dataSnapshot: DataSnapshot) {
                                    if (dataSnapshot.value == null) {
                                        statsRef.child(user.uid).child("misses").setValue(1)
                                    } else {
                                        val newValue = dataSnapshot.getValue(Int::class.java)!! + 1
                                        statsRef.child(user.uid).child("misses")
                                            .setValue(newValue)
                                    }
                                }

                                override fun onCancelled(databaseError: DatabaseError) {
                                    Log.d("SSS", "STATS READ FAILED", databaseError.toException())
                                }
                            })
                    }
                    CellType.HIT -> {
                        statsRef
                            .child(user.uid)
                            .child("hits")
                            .addListenerForSingleValueEvent(object : ValueEventListener {
                                override fun onDataChange(dataSnapshot: DataSnapshot) {
                                    if (dataSnapshot.value == null) {
                                        statsRef.child(user.uid).child("hits").setValue(1)
                                    } else {
                                        val newValue = dataSnapshot.getValue(Int::class.java)!! + 1
                                        statsRef.child(user.uid).child("hits")
                                            .setValue(newValue)
                                    }
                                }

                                override fun onCancelled(databaseError: DatabaseError) {
                                    Log.d("SSS", "STATS READ FAILED", databaseError.toException())
                                }
                            })
                    }
                }
            }
        }

        fun upload(hit: Hit) {
            if (enemyUid.isNotEmpty()) {
                val key = currentGameRef
                    .child("players")
                    .child(enemyUid)
                    .child("hits")
                    .push()
                    .key!!
                currentGameRef
                    .child("players")
                    .child(enemyUid)
                    .child("hits")
                    .child(key)
                    .setValue(hit)
            }
        }

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
                setTurn(it.uid)
                setWin("No one")
            }
        }

        fun setTurn(userUid: String) {
            if (userUid.isNotEmpty()) {
                currentGameRef
                    .child("turn")
                    .setValue(userUid)
            }
        }

        fun setWin(userUid: String) {
            if (userUid.isNotEmpty()) {
                currentGameRef
                    .child("win")
                    .setValue(userUid)
            }
        }

        fun removeGame() = currentGameRef.removeValue()

        fun joinGame(code: String, user: FirebaseUser?, isGameExists: (Boolean) -> Unit) {
            if (code.isEmpty()) {
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