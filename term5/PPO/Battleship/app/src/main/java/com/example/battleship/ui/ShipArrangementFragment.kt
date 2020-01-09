package com.example.battleship.ui

import android.annotation.SuppressLint
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.ProgressBar
import android.widget.TextView
import android.widget.Toast
import androidx.annotation.RequiresApi
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.BattlefieldView
import com.example.battleship.R
import com.example.battleship.models.CellType
import com.example.battleship.models.Direction
import com.example.battleship.models.Rank
import com.example.battleship.models.Ship
import com.example.battleship.service.FirebaseService
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener

class ShipArrangementFragment: Fragment() {

    private lateinit var battlefieldView: BattlefieldView
    private lateinit var continueGameButton: Button
    private lateinit var hintTextView: TextView
    private lateinit var horizontalButton: Button
    private lateinit var verticalButton: Button
    private lateinit var continueProgressBar: ProgressBar

    private var playersListener: ValueEventListener? = null

    private var currentDirection = Direction.HORIZONTAL
    private var currentRank = Rank.AIRCRAFT_CARRIER

    private val shipsToBePlaced = mutableListOf(
        Ship(Rank.AIRCRAFT_CARRIER),
        Ship(Rank.BATTLESHIP),
        Ship(Rank.CRUISER),
        Ship(Rank.DESTROYER),
        Ship(Rank.DESTROYER),
        Ship(Rank.SUBMARINE),
        Ship(Rank.SUBMARINE)
    )

    private var allShipsArePlaced = false

    private val ships = mutableListOf<Ship>()

    override fun onDestroyView() {
        super.onDestroyView()
        playersListener?.let {
            FirebaseService
                .currentGameRef
                .child("ready")
                .removeEventListener(it)
        }
    }

    @RequiresApi(Build.VERSION_CODES.LOLLIPOP)
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_ship_arrangement, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun hit(i: Int, j: Int) {

        if (shipsToBePlaced.isEmpty() && allShipsArePlaced) { return }

        when (battlefieldView.cells[i][j].type) {

            CellType.SHIP -> {
                Toast.makeText(
                        requireContext(),
                    "Ship can't be placed here",
                    Toast.LENGTH_SHORT
                ).show()
            }
            CellType.WATER -> {
                val ship = Ship(currentRank, currentDirection, i, j)
                battlefieldView.draw(ship) { willDraw ->
                    if (!willDraw) {
                        Toast.makeText(
                            requireContext(),
                            "Ship can't be placed here",
                            Toast.LENGTH_SHORT
                        ).show()
                    } else {
                        ships.add(ship)
                        placingShips()
                    }
                }
            }
        }
    }

    @RequiresApi(Build.VERSION_CODES.LOLLIPOP)
    @SuppressLint("ClickableViewAccessibility")
    private fun setupLayoutFor(view: View) {

        (activity as MainActivity).snowfallView.stopFalling()

        battlefieldView = view.findViewById(R.id.battlefieldView)

        battlefieldView.setOnTouchListener { v, event ->
            val x = event.x
            val y = event.y
            val rect = battlefieldView.getRectIndexesFor(x, y)
            hit(rect.first, rect.second)
            v?.onTouchEvent(event) ?: true
        }

        continueGameButton = view.findViewById(R.id.continueGameButton)
        continueGameButton.setOnClickListener {
            val action = ShipArrangementFragmentDirections
                .actionShipArrangementFragmentToGameFragment()
            if (findNavController().currentDestination?.id == R.id.shipArrangementFragment) {
                findNavController().navigate(action)
            }
        }

        continueProgressBar = view.findViewById(R.id.continueProgressBar)

        hintTextView = view.findViewById(R.id.hintTextView)

        horizontalButton = view.findViewById(R.id.btnHorizontal)
        horizontalButton.setOnClickListener {
            toggleButtons()
        }

        verticalButton = view.findViewById(R.id.btnVertical)
        verticalButton.setOnClickListener {
            toggleButtons()
        }

        playersListener = object: ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                if (dataSnapshot.childrenCount.toInt() == 2) {
                    continueProgressBar.visibility = View.GONE
                    continueGameButton.isEnabled = true
                    continueGameButton.setBackgroundTintList(resources.getColorStateList(R.color.colorAccent))
                } else {
                    continueProgressBar.visibility = View.VISIBLE
                    continueGameButton.isEnabled = false
                    continueGameButton.setBackgroundTintList(resources.getColorStateList(R.color.colorPrimary))
                }
            }

            override fun onCancelled(databaseError: DatabaseError) {
                Log.w("SSS", "GAME CONNECTION CANCELED", databaseError.toException())
            }
        }

        FirebaseService
            .currentGameRef
            .child("ready")
            .addValueEventListener(playersListener!!)

        placingShips()
    }

    private fun placingShips() {
        if (shipsToBePlaced.isNotEmpty()) {
            val ship = shipsToBePlaced.first()
            currentRank = ship.rank
            hintTextView.text = "Place ${currentRank.size} cell ship"
            shipsToBePlaced.removeAt(shipsToBePlaced.indexOf(ship))
        } else {
            hintTextView.text = "No more ships to place"
            allShipsArePlaced = true
            FirebaseService.continueGame(FirebaseService.auth.currentUser)
            FirebaseService.upload(ships, FirebaseService.auth.currentUser)
        }
    }

    @RequiresApi(Build.VERSION_CODES.LOLLIPOP)
    private fun toggleButtons() {
        if (horizontalButton.isEnabled) {
            horizontalButton.isEnabled = false
            horizontalButton.setBackgroundTintList(resources.getColorStateList(R.color.colorAccent))
            verticalButton.isEnabled = true
            verticalButton.setBackgroundTintList(resources.getColorStateList(R.color.colorPrimary))
            currentDirection = Direction.HORIZONTAL

        } else {
            horizontalButton.isEnabled = true
            horizontalButton.setBackgroundTintList(resources.getColorStateList(R.color.colorPrimary))
            verticalButton.isEnabled = false
            verticalButton.setBackgroundTintList(resources.getColorStateList(R.color.colorAccent))
            currentDirection = Direction.VERTICAL
        }
    }
}