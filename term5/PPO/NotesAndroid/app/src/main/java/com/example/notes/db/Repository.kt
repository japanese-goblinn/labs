package com.example.notes.db

import com.example.notes.interfaces.NotesDao
import com.example.notes.interfaces.NotesTagsDao
import com.example.notes.interfaces.TagsDao
import com.example.notes.models.Note

class Repository(
    private val notesDao: NotesDao,
    private val tagsDao: TagsDao,
    private val notesTagsDao: NotesTagsDao
) {
    val notes = notesDao.getAll()

    suspend fun add(note: Note) = notesDao.insert(note)

    suspend fun delete(note: Note) = notesDao.delete(note)

    suspend fun findBy(id: Int): Note? = notesDao.findBy(id)

    suspend fun update(note: Note) {
        delete(note)
        add(note)
    }
}