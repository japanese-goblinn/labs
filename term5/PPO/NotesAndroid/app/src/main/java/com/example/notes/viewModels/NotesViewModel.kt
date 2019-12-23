package com.example.notes.viewModels

import android.app.Application
import android.util.Log
import androidx.lifecycle.*
import com.example.notes.db.Database
import com.example.notes.db.Repository
import com.example.notes.models.Note

class NotesViewModel(application: Application): AndroidViewModel(application) {

    private val repository: Repository

    val notes: LiveData<List<Note>>

    init {
        val db = Database.get(application, viewModelScope)
        Log.d("SSS", "DB CREATED")
        repository = Repository(db.notesDao(), db.tagsDao())
        notes = repository.notes
    }

}