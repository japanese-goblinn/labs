package com.example.notes.viewModels

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import com.example.notes.db.Database
import com.example.notes.db.Repository
import com.example.notes.models.Note
import kotlinx.coroutines.async
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking

class NoteDetailViewModel(application: Application): AndroidViewModel(application) {

    private val repository: Repository

    init {
        val db = Database.get(application, viewModelScope)
        repository = Repository(db.notesDao(), db.tagsDao(), db.notesTagsDao())
    }

    fun findBy(id: Int): Note? = runBlocking {
        return@runBlocking repository.findBy(id)
    }

    fun add(note: Note) = viewModelScope.launch { repository.add(note) }

    fun update(note: Note) = viewModelScope.launch { repository.update(note) }
}