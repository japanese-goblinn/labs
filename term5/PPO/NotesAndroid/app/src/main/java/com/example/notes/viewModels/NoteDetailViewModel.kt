package com.example.notes.viewModels

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import com.example.notes.db.Database
import com.example.notes.db.Repository
import com.example.notes.models.Note
import com.example.notes.models.NoteTag
import com.example.notes.models.Tag
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

    fun tagsFor(note: Note) = runBlocking {
        return@runBlocking repository.tagsIdsFor(note)
            .mapNotNull { repository.findTagBy(it)?.name }
    }

    fun add(note: Note, tags: List<String>) = runBlocking {
        val noteId = repository.add(note)
        if (tags.isNotEmpty()) {
            updateTags(noteId, tags)
        }
    }

    fun update(note: Note, tags: List<String>) = runBlocking {
        val noteId = repository.update(note)
        if (tags.isNotEmpty()) {
            updateTags(noteId, tags)
        }
    }

    private suspend fun updateTags(noteId: Int, tags: List<String>) {
        val newTags = tags.toSet()
        val allTags = repository.allTags().toSet()

        val newTagsToAdd = newTags.minus(allTags)
        val newTagsToAddIDs = mutableListOf<Int>()
        for (tag in newTagsToAdd) {
            newTagsToAddIDs.add(
                repository.add(
                    Tag(name = tag.toString())
                )
            )
        }
        for (tagId in newTagsToAddIDs) {
            repository.add(
                NoteTag(noteId = noteId, tagId = tagId)
            )
        }

        val commonWithAllTags = allTags intersect newTags
        val commonTagsToAddIDs = mutableListOf<Int>()
        for (tag in commonWithAllTags) {
            commonTagsToAddIDs.add(
                repository.findTagId(tag.toString())
            )
        }
        for (tagId in commonTagsToAddIDs) {
            repository.add(
                NoteTag(noteId = noteId, tagId = tagId)
            )
        }
    }
}