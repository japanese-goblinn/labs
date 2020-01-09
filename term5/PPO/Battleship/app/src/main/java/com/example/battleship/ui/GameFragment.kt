package com.example.battleship.ui

import android.annotation.SuppressLint
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.views.BattlefieldView
import com.example.battleship.R
import com.example.battleship.models.CellType
import com.example.battleship.models.Hit
import com.example.battleship.models.Ship
import com.example.battleship.service.FirebaseService
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener

class GameFragment: Fragment() {

    private lateinit var statusTextView: TextView
    private lateinit var myBattlefieldView: BattlefieldView
    private lateinit var enemyBattlefieldView: BattlefieldView

    private var myShipsValueEventListener: ValueEventListener? = null
    private var enemyShipsValueEventListener: ValueEventListener? = null
    private var hitsValueEventListener: ValueEventListener? = null
    private var whosTurnEventListener: ValueEventListener? = null
    private var winEventListener: ValueEventListener? = null

    private val myShips = mutableListOf<Ship>()
    private val enemyShips = mutableListOf<Ship>()
    private val hits = mutableListOf<Hit>()

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_game, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    override fun onDestroyView() {
        super.onDestroyView()
        myShipsValueEventListener?.let {
            FirebaseService
                .currentGameRef
                .child("players")
                .child(FirebaseService.auth.currentUser!!.uid)
                .child("ships")
                .removeEventListener(it)
        }
        myShipsValueEventListener?.let {
            FirebaseService
                .currentGameRef
                .child("players")
                .child(FirebaseService.enemyUid)
                .child("ships")
                .removeEventListener(it)
        }
        hitsValueEventListener?.let {
            FirebaseService
                .currentGameRef
                .child("players")
                .child(FirebaseService.enemyUid)
                .child("hits")
                .removeEventListener(it)
        }
        whosTurnEventListener?.let {
            FirebaseService
                .currentGameRef
                .child("turn")
                .removeEventListener(it)
        }
        winEventListener?.let {
            FirebaseService
                .currentGameRef
                .child("win")
                .removeEventListener(it)
        }
    }

    private fun hit(i: Int, j: Int) {
        when (enemyBattlefieldView.cells[i][j].type) {
            CellType.WATER -> {
                enemyBattlefieldView.drawMiss(i, j)
                val miss = Hit(CellType.MISS, i, j)
                FirebaseService.upload(miss)
                FirebaseService.statsUpload(miss.type)
                FirebaseService.setTurn(FirebaseService.enemyUid)
            }
            CellType.SHIP -> {
                enemyBattlefieldView.drawHit(i, j)
                val hit = Hit(CellType.HIT, i, j)
                FirebaseService.upload(hit)
                FirebaseService.statsUpload(hit.type)
                FirebaseService.setTurn(FirebaseService.auth.currentUser!!.uid)
            }
        }
        if (isWin()) {
            FirebaseService.setWin(FirebaseService.auth.currentUser!!.uid)
        }
    }

    private fun isWin(): Boolean {
        val isWin = mutableListOf<Boolean>()
        for (cellRow in enemyBattlefieldView.cells) {
            for (cell in cellRow) {
                isWin.add(cell.type != CellType.SHIP)
            }
        }
        return isWin.all { it == true }
    }

    @SuppressLint("ClickableViewAccessibility")
    private fun setupLayoutFor(view: View) {

        myBattlefieldView = view.findViewById(R.id.myBattlefieldView)
        enemyBattlefieldView = view.findViewById(R.id.enemyBattlefieldView)
        enemyBattlefieldView.setOnTouchListener { v, event ->
            val x = event.x
            val y = event.y
            val rect = enemyBattlefieldView.getRectIndexesFor(x, y)
            hit(rect.first, rect.second)
            v?.onTouchEvent(event) ?: true
        }

        statusTextView = view.findViewById(R.id.statusTextView)

        myShipsValueEventListener = object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                dataSnapshot.children.mapNotNullTo(myShips) { it.getValue(Ship::class.java) }
                myShips.forEach {
                    myBattlefieldView.draw(it)
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                Log.d("SSS", "MY SHIPS LOAD FAILED")
            }
        }

        enemyShipsValueEventListener = object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                dataSnapshot.children.mapNotNullTo(enemyShips) { it.getValue(Ship::class.java) }
                enemyBattlefieldView.isEnemyView = true
                enemyShips.forEach {
                    enemyBattlefieldView.draw(it)
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                Log.d("SSS", "ENEMY SHIPS LOAD FAILED")
            }
        }

        hitsValueEventListener = object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                dataSnapshot.children.mapNotNullTo(hits) { it.getValue(Hit::class.java) }
                hits.forEach {
                    when (it.type) {
                        CellType.HIT -> {
                            myBattlefieldView.drawHit(it.i, it.j)
                        }
                        CellType.MISS -> {
                            myBattlefieldView.drawMiss(it.i, it.j)
                        }
                    }
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                Log.d("SSS", "ENEMY SHIPS LOAD FAILED")
            }
        }

        whosTurnEventListener = object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                val uid = dataSnapshot.getValue(String::class.java)
                if (uid == FirebaseService.auth.currentUser!!.uid) {
                    statusTextView.text = "Your turn!"
                    enemyBattlefieldView.isEnabled = true
                } else {
                    statusTextView.text = "Enemies turn!"
                    enemyBattlefieldView.isEnabled = false
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                Log.d("SSS", "WHOUS TURN LOAD FAILED")
            }
        }

        winEventListener = object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                val uid = dataSnapshot.getValue(String::class.java)
                if (uid == FirebaseService.auth.currentUser!!.uid) {
                    val action = GameFragmentDirections
                        .actionGameFragmentToResultFragment("win")
                    findNavController().navigate(action)
                } else if (uid == FirebaseService.enemyUid) {
                    val action = GameFragmentDirections
                        .actionGameFragmentToResultFragment("lose")
                    findNavController().navigate(action)
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                Log.d("SSS", "WIN LOAD FAILED")
            }
        }

        FirebaseService
            .currentGameRef
            .child("players")
            .child(FirebaseService.auth.currentUser!!.uid)
            .child("ships")
            .addListenerForSingleValueEvent(myShipsValueEventListener!!)

        FirebaseService
            .currentGameRef
            .child("players")
            .child(FirebaseService.enemyUid)
            .child("ships")
            .addListenerForSingleValueEvent(enemyShipsValueEventListener!!)

        FirebaseService
            .currentGameRef
            .child("players")
            .child(FirebaseService.auth.currentUser!!.uid)
            .child("hits")
            .addValueEventListener(hitsValueEventListener!!)

        FirebaseService
            .currentGameRef
            .child("turn")
            .addValueEventListener(whosTurnEventListener!!)

        FirebaseService
            .currentGameRef
            .child("win")
            .addValueEventListener(winEventListener!!)
    }
}