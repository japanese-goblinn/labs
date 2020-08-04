package com.example.battleship.ui

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.R
import com.example.battleship.service.FirebaseService
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener

class StatisticsFragment: Fragment() {

    private lateinit var backButton: ImageButton
    private lateinit var hitsTextView: TextView
    private lateinit var missesTextView: TextView
    private lateinit var winsTextView: TextView
    private lateinit var losesTextView: TextView

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_statistics, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(view: View) {
        backButton = view.findViewById(R.id.statisticsBackButton)
        backButton.setOnClickListener { findNavController().popBackStack() }

        hitsTextView = view.findViewById(R.id.hitsTextView)
        missesTextView = view.findViewById(R.id.missesTextView)
        winsTextView = view.findViewById(R.id.winsTextView)
        losesTextView = view.findViewById(R.id.losesTextView)

        FirebaseService.auth.currentUser?.let { user ->
            FirebaseService.statsRef
                .child(user.uid)
                .child("wins")
                .addListenerForSingleValueEvent(object : ValueEventListener {
                    override fun onDataChange(dataSnapshot: DataSnapshot) {
                        dataSnapshot.getValue(Int::class.java)?.let {
                            winsTextView.text = "TOTAL WINS: $it"
                        }
                    }

                    override fun onCancelled(databaseError: DatabaseError) {
                        Log.d("SSS", "STATS READ FAILED", databaseError.toException())
                    }
                })
            FirebaseService.statsRef
                .child(user.uid)
                .child("loses")
                .addListenerForSingleValueEvent(object : ValueEventListener {
                    override fun onDataChange(dataSnapshot: DataSnapshot) {
                        dataSnapshot.getValue(Int::class.java)?.let {
                            losesTextView.text = "TOTAL LOSES: $it"
                        }
                    }

                    override fun onCancelled(databaseError: DatabaseError) {
                        Log.d("SSS", "STATS READ FAILED", databaseError.toException())
                    }
                })
            FirebaseService.statsRef
                .child(user.uid)
                .child("hits")
                .addListenerForSingleValueEvent(object : ValueEventListener {
                    override fun onDataChange(dataSnapshot: DataSnapshot) {
                        dataSnapshot.getValue(Int::class.java)?.let {
                            hitsTextView.text = "TOTAL HITS: $it"
                        }
                    }

                    override fun onCancelled(databaseError: DatabaseError) {
                        Log.d("SSS", "STATS READ FAILED", databaseError.toException())
                    }
                })
            FirebaseService.statsRef
                .child(user.uid)
                .child("misses")
                .addListenerForSingleValueEvent(object : ValueEventListener {
                    override fun onDataChange(dataSnapshot: DataSnapshot) {
                        dataSnapshot.getValue(Int::class.java)?.let {
                            missesTextView.text = "TOTAL MISSES: $it"
                        }
                    }

                    override fun onCancelled(databaseError: DatabaseError) {
                        Log.d("SSS", "STATS READ FAILED", databaseError.toException())
                    }
                })
        }
    }
}