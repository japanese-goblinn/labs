package com.example.notes.ui.fragments

import android.content.Context
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.InputMethodManager
import android.widget.EditText
import androidx.annotation.RequiresApi
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
import kotlinx.coroutines.runBlocking
import java.time.LocalDate
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter
import java.util.*

class NoteDetailFragment: Fragment() {

    private val args: NoteDetailFragmentArgs by navArgs()
    private lateinit var viewModel: NoteDetailViewModel
    private var note: Note? = null

    private lateinit var titleEditText: TextInputEditText
    private lateinit var contentEditText: EditText
    private lateinit var tagsEditText: TextInputEditText
    private lateinit var fabSaveButton: FloatingActionButton

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_note_detail, container, false)
        setupLayoutFor(rootView)
        return rootView
    }

    @RequiresApi(Build.VERSION_CODES.O)
    private fun setupLayoutFor(rootView: View) {

        viewModel = ViewModelProviders.of(this).get(NoteDetailViewModel::class.java)

        titleEditText = rootView.findViewById(R.id.titleTextInputEditText)
        contentEditText = rootView.findViewById(R.id.contentEditText)
        tagsEditText = rootView.findViewById(R.id.tagsTextInputEditText)

        if (args.noteId != -1) {
            viewModel.findBy(args.noteId)?.let { note = it }
            titleEditText.setText(note?.title)
            contentEditText.setText(note?.content)
            tagsEditText.setText(viewModel.tagsFor(note)?.joinToString(" "))
        }

        fabSaveButton = rootView.findViewById(R.id.fabSave)

        fabSaveButton.setOnClickListener {
            val title: String?
            if (titleEditText.text?.isEmpty() == true) {
                title = format(LocalDateTime.now())
            } else {
                title = titleEditText.text.toString()
            }
            val content = contentEditText.text.toString()
            if (note == null) {
                val note = Note(
                    title = title,
                    content = content,
                    lastUpdate = Date()
                )
                viewModel.add(note, getUniqueTags())
            } else {
                note?.lastUpdate = Date()
                note?.title = title
                note?.content = content
                viewModel.update(note, getUniqueTags())
            }
            findNavController().navigate(R.id.action_noteDetailFragment_to_notesListFragment)
        }
    }

    @RequiresApi(Build.VERSION_CODES.O)
    private fun format(date: LocalDateTime): String {
        val formatter = DateTimeFormatter.ofPattern("dd.MM.yyyy HH:mm")
        return date.format(formatter)
    }

    private fun getUniqueTags(): List<String> = tagsEditText.text
        .toString()
        .split(" ")
        .filter { it != "" }
        .distinct()
}