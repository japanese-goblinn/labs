package com.example.battleship.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.example.battleship.R

class BattlefieldFragment: Fragment() {

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_battlefield, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(view: View) {

    }
}