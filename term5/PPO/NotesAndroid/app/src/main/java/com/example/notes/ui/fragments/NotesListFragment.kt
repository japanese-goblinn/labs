package com.example.notes.ui.fragments

import android.content.res.Resources
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.LinearLayout
import androidx.core.content.ContextCompat
import androidx.core.content.res.ResourcesCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.notes.R
import com.example.notes.ui.recycler.NotesAdapter
import com.example.notes.viewModels.NotesViewModel
import com.google.android.material.floatingactionbutton.FloatingActionButton

class NotesListFragment: Fragment() {

    private lateinit var viewModel: NotesViewModel
    private lateinit var notesList: RecyclerView
    private lateinit var viewAdapter: NotesAdapter
    private lateinit var fab: FloatingActionButton

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

        notesList = rootView.findViewById(R.id.notesList)

        viewModel = ViewModelProviders.of(this).get(NotesViewModel::class.java)

        viewAdapter = NotesAdapter(requireContext())
        notesList.adapter = viewAdapter
        notesList.layoutManager = LinearLayoutManager(requireContext())
        val decoration = DividerItemDecoration(requireContext(), DividerItemDecoration.VERTICAL)
        decoration.setDrawable(ContextCompat.getDrawable(requireContext(), R.drawable.divider)!!)
        notesList.addItemDecoration(decoration)

        viewModel.notes.observe(viewLifecycleOwner, Observer {
            viewAdapter.setupNotes(it.toMutableList())
        })

//        fab = rootView.findViewById(R.id.fab)
//        fab.setOnClickListener {
//
//        }

    }

}