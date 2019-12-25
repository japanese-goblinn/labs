package com.example.notes.viewModels

import android.app.Application
import android.os.Bundle
import android.util.Log
import androidx.lifecycle.*
import com.example.notes.db.Database
import com.example.notes.db.Repository
import com.example.notes.models.Note
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking

enum class SortOrder {
    DATE, TITLE
}

class NotesListViewModel(application: Application): AndroidViewModel(application) {

    private var currentSortOrder = SortOrder.DATE

    private val repository: Repository
    private val dbNotes: LiveData<List<Note>>

    val notes = MediatorLiveData<List<Note>>()

    init {
        val db = Database.get(application, viewModelScope)
        repository = Repository(db.notesDao(), db.tagsDao(), db.notesTagsDao())
        dbNotes = repository.notes

        notes.addSource(dbNotes) {
            notes.value = dbNotes.value
            sortNotes(currentSortOrder)
        }

    }

    fun sortNotes(order: SortOrder) = when (order) {
        SortOrder.DATE -> {
            currentSortOrder = order
            notes.value = dbNotes.value?.sortedByDescending { it.lastUpdate  }
        }
        SortOrder.TITLE -> {
            currentSortOrder = order
            notes.value =  dbNotes.value?.sortedBy { it.title }
        }
    }

    fun search(query: String?) {
        if (query.isNullOrEmpty()) {
           sortNotes(currentSortOrder)
        } else {
            when (currentSortOrder) {
                SortOrder.DATE -> {
                    notes.value = notesFor(query).value?.sortedByDescending { it.lastUpdate }
                }
                SortOrder.TITLE -> {
                    notes.value = notesFor(query).value?.sortedBy { it.title }
                }
            }
        }
    }

    private fun notesFor(tag: String): MutableLiveData<List<Note>> = runBlocking {
        val tagId = repository.findTagId(tag)
        tagId?.let {
            val notes = repository
                .notesIdsFor(tagId)
                .mapNotNull { repository.findBy(it) }
            return@runBlocking MutableLiveData<List<Note>>(notes)
        }
        return@runBlocking MutableLiveData<List<Note>>()
    }

    fun delete(note: Note) = viewModelScope.launch { repository.delete(note) }

    fun tagsFor(note: Note) = runBlocking {
        return@runBlocking repository.tagsIdsFor(note)
            .mapNotNull { repository.findTagBy(it)?.name }
    }
}