package com.example.battleship.ui

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.example.battleship.R
import com.example.battleship.service.FirebaseService

class GameFragment: Fragment() {

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_game, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(view: View) {
        Log.d("SSS", FirebaseService.enemyUid)

        

        //        val menuListener = object : ValueEventListener {
//            override fun onDataChange(dataSnapshot: DataSnapshot) {
//                dataSnapshot.children.mapNotNullTo(myShips) { it.getValue<Ship>(Ship::class.java) }
//                Log.d("SSS", "SSS")
//            }
//
//            override fun onCancelled(databaseError: DatabaseError) {
////                println("loadPost:onCancelled ${databaseError.toException()}")
//            }
//        }
//        FirebaseService
//            .currentGameRef
//            .child("players")
//            .child(FirebaseService.auth.currentUser!!.uid)
//            .child("ships").addListenerForSingleValueEvent(menuListener)
    }
}