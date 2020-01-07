package com.example.battleship.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import com.example.battleship.R

class StatisticsFragment: Fragment() {

    private lateinit var backButton: ImageButton

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
    }
}