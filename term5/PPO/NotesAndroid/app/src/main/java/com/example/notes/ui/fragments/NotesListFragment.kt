package com.example.notes.ui.fragments

import android.content.res.Configuration
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.PopupMenu
import android.widget.SearchView
import android.widget.TextView
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import androidx.navigation.Navigation
import androidx.navigation.fragment.findNavController
import androidx.recyclerview.widget.*
import com.example.notes.R
import com.example.notes.models.Note
import com.example.notes.ui.recycler.NotesAdapter
import com.example.notes.viewModels.NotesListViewModel
import com.example.notes.viewModels.SortOrder
import com.google.android.material.floatingactionbutton.FloatingActionButton
import kotlinx.android.synthetic.main.fragment_notes_list.*

class NotesListFragment: Fragment() {

    private companion object {
        const val SORTING = "SORTING"
    }

    private lateinit var viewModel: NotesListViewModel
    private lateinit var notesList: RecyclerView
    private lateinit var viewAdapter: NotesAdapter
    private lateinit var fab: FloatingActionButton
    private lateinit var sortButton: ImageButton
    private lateinit var sortTextView: TextView
    private lateinit var searchBarView: SearchView

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        outState.putString(SORTING, sortTextView.text.toString())
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        savedInstanceState?.let {
            sortTextView.text = it.getString(SORTING)
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(
            R.layout.fragment_notes_list, container, false
        )
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
            val decoration = DividerItemDecoration(
                requireContext(),
                DividerItemDecoration.VERTICAL
            )
            decoration.setDrawable(
                ContextCompat.getDrawable(requireContext(), R.drawable.divider)!!
            )
            notesList.addItemDecoration(decoration)
        }

        viewAdapter.onItemClick = {
            val action = NotesListFragmentDirections
                .actionNotesListFragmentToNoteDetailFragment(it.id, "Note Details")
            findNavController().navigate(action)
        }

        viewAdapter.onDeleteButtonClick = {
            viewModel.delete(it)
        }

        viewModel.notes.observe(
            viewLifecycleOwner,
            Observer { notes: List<Note>? ->
                viewAdapter.setupNotes(notes)
            }
        )

        fab = rootView.findViewById(R.id.fab)
        fab.setOnClickListener {
            val action = NotesListFragmentDirections
                .actionNotesListFragmentToNoteDetailFragment(-1, "New Note")
            findNavController().navigate(action)
        }

        sortTextView = rootView.findViewById(R.id.sortingTextField)
        sortTextView.text = "Sorting By Date"

        sortButton = rootView.findViewById(R.id.sortButton)
        sortButton.setOnClickListener {
            val popup = PopupMenu(requireContext(), it)
            popup.inflate(R.menu.popup_menu)

            popup.setOnMenuItemClickListener { menuItem ->
                when (menuItem.itemId) {
                    R.id.sortByDate -> {
                        sortTextView.text = "Sorting By Date"
                        viewModel.sortNotes(SortOrder.DATE)
                    }
                    R.id.sortByTitle -> {
                        sortTextView.text = "Sorting By Title"
                        viewModel.sortNotes(SortOrder.TITLE)
                    }
                }
                return@setOnMenuItemClickListener true
            }
            popup.show()
        }

        searchBarView = rootView.findViewById(R.id.searchBar)
        searchBarView.setOnQueryTextListener(object: SearchView.OnQueryTextListener {

            override fun onQueryTextChange(newText: String?): Boolean {
                viewModel.search(newText)
                return false
            }

            override fun onQueryTextSubmit(query: String?): Boolean {
                viewModel.search(query)
                return false
            }
        })
    }

}