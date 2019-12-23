package com.example.notes.db

import com.example.notes.interfaces.NotesDao
import com.example.notes.interfaces.TagsDao

class Repository(
    private val notesDao: NotesDao,
    private val tagsDao: TagsDao
) {
    val notes = notesDao.getAll()

    val tags = tagsDao.getAll()
}