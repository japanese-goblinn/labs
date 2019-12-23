package com.example.notes.interfaces

import androidx.lifecycle.LiveData
import androidx.room.*
import com.example.notes.models.Note
import com.example.notes.models.NoteTag

@Dao
interface NotesTagsDao {

    @Insert
    suspend fun insert(noteTag: NoteTag)

    @Delete
    fun delete(noteTag: NoteTag)

    @Query("DELETE FROM notes_tags")
    suspend fun deleteAll()
}