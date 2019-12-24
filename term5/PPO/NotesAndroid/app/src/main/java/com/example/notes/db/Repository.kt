package com.example.notes.db

import com.example.notes.interfaces.NotesDao
import com.example.notes.interfaces.NotesTagsDao
import com.example.notes.interfaces.TagsDao
import com.example.notes.models.Note
import com.example.notes.models.NoteTag
import com.example.notes.models.Tag

class Repository(
    private val notesDao: NotesDao,
    private val tagsDao: TagsDao,
    private val notesTagsDao: NotesTagsDao
) {
    val notes = notesDao.getAll()

    suspend fun add(note: Note): Int = notesDao.insert(note).toInt()

    suspend fun add(tag: Tag): Int = tagsDao.insert(tag).toInt()

    suspend fun add(noteTag: NoteTag) = notesTagsDao.insert(noteTag)

    suspend fun delete(note: Note) = notesDao.delete(note)

    suspend fun findBy(id: Int): Note? = notesDao.findBy(id)

    //
    suspend fun update(note: Note): Int {
        delete(note)
        return add(note)
    }

    suspend fun tagsIdsFor(note: Note): List<Int> = notesTagsDao.tagsIdsFor(note.id)

    suspend fun findTagBy(id: Int): Tag? = tagsDao.findBy(id)

    suspend fun findTagId(name: String) = tagsDao.findIdBy(name)

    suspend fun allTags(): List<Tag> = tagsDao.getAll()

}