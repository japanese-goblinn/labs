package com.example.notes.viewModels

import android.app.Application
import android.os.Bundle
import android.util.Log
import androidx.lifecycle.*
import com.example.notes.db.Database
import com.example.notes.db.Repository
import com.example.notes.models.Note
import kotlinx.coroutines.launch

class NotesListViewModel(application: Application): AndroidViewModel(application) {

    private val repository: Repository

    val notes: LiveData<List<Note>>

    init {
        val db = Database.get(application, viewModelScope)
        repository = Repository(db.notesDao(), db.tagsDao(), db.notesTagsDao())
        notes = repository.notes
    }

    fun delete(note: Note) = viewModelScope.launch { repository.delete(note) }

}