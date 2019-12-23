package com.example.notes.ui.fragments

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.InputMethodManager
import android.widget.EditText
import androidx.core.content.ContextCompat.getSystemService
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProviders
import androidx.navigation.fragment.findNavController
import androidx.navigation.fragment.navArgs
import com.example.notes.R
import com.example.notes.models.Note
import com.example.notes.viewModels.NoteDetailViewModel
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.textfield.TextInputEditText
import java.util.*

class NoteDetailFragment: Fragment() {

    private val args: NoteDetailFragmentArgs by navArgs()
    private lateinit var viewModel: NoteDetailViewModel
    private var note: Note? = null

    private lateinit var titleEditText: TextInputEditText
    private lateinit var contentEditText: EditText
    private lateinit var tagsEditText: TextInputEditText
    private lateinit var fabSaveButton: FloatingActionButton

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_note_detail, container, false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(rootView: View) {

        viewModel = ViewModelProviders.of(this).get(NoteDetailViewModel::class.java)

        titleEditText = rootView.findViewById(R.id.titleTextInputEditText)
        contentEditText = rootView.findViewById(R.id.contentEditText)
        tagsEditText = rootView.findViewById(R.id.tagsTextInputEditText)

        if (args.noteId != -1) {
            viewModel.findBy(args.noteId)?.let { note = it }
            titleEditText.setText(note?.title)
            contentEditText.setText(note?.content)
        }

        fabSaveButton = rootView.findViewById(R.id.fabSave)

        fabSaveButton.setOnClickListener {
            val title = titleEditText.text.toString()
            val content: String?
            if (contentEditText.text.isEmpty()) {
                content = null
            } else {
                content = contentEditText.text.toString()
            }
            if (note == null) {
                val note = Note(
                    title = title,
                    content = content,
                    lastUpdate = Date()
                )
                viewModel.add(note)
            } else {
                note?.lastUpdate = Date()
                note?.title = title
                note?.content = content
                viewModel.update(note!!)
            }
            findNavController().navigate(R.id.action_noteDetailFragment_to_notesListFragment)
        }
    }

}