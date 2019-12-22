package com.example.notes

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Adapter
import android.widget.Button
import android.widget.ListAdapter
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView

class NotesListFragment: Fragment() {

    private lateinit var notesList: RecyclerView
    private lateinit var viewAdapter: Adapter
    private lateinit var viewManager: LinearLayoutManager

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_notes_list, container, false)
        setupLayoutFor(view)
        return view
    }

    private fun setupLayoutFor(rootView: View) {
//        notesList = rootView.findViewById(R.id.notesList)
//        notesList.apply {
//            viewManager = LinearLayoutManager(activity)
//            viewAdapter = ListAdapter()
//        }
    }

}