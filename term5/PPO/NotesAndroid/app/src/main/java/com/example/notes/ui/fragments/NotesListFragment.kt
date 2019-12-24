package com.example.notes.ui.fragments

import android.content.res.Configuration
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import androidx.navigation.fragment.findNavController
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.notes.R
import com.example.notes.models.Note
import com.example.notes.ui.recycler.NotesAdapter
import com.example.notes.viewModels.NotesListViewModel
import com.google.android.material.floatingactionbutton.FloatingActionButton

class NotesListFragment: Fragment() {

    private lateinit var viewModel: NotesListViewModel
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

        viewModel = ViewModelProviders.of(this).get(NotesListViewModel::class.java)

        viewAdapter = NotesAdapter(requireContext(), viewModel)
        notesList.adapter = viewAdapter

        val orientation = activity?.resources?.configuration?.orientation
        if (orientation == Configuration.ORIENTATION_LANDSCAPE) {
            notesList.layoutManager = GridLayoutManager(requireContext(), 3)
        } else {
            notesList.layoutManager = LinearLayoutManager(requireContext())
            val decoration = DividerItemDecoration(requireContext(), DividerItemDecoration.VERTICAL)
            decoration.setDrawable(ContextCompat.getDrawable(requireContext(), R.drawable.divider)!!)
            notesList.addItemDecoration(decoration)
        }

        viewAdapter.onItemClick = {
            val action = NotesListFragmentDirections
                .actionNotesListFragmentToNoteDetailFragment(it.id)
            findNavController().navigate(action)
        }

        viewAdapter.onDeleteButtonClick = {
            viewModel.delete(it)
        }

        viewModel.notes.observe(viewLifecycleOwner, Observer {
            viewAdapter.setupNotes(it.toMutableList())
        })

        fab = rootView.findViewById(R.id.fab)
        fab.setOnClickListener {
            val action = NotesListFragmentDirections
                .actionNotesListFragmentToNoteDetailFragment(-1)
            findNavController().navigate(action)
        }

    }

}