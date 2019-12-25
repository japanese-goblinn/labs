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

    @Query("SELECT tag_id FROM notes_tags WHERE note_id = :noteId")
    suspend fun tagsIdsFor(noteId: Int): List<Int>

    @Query("SELECT note_id FROM notes_tags WHERE tag_id = :tagId")
    suspend fun notesIdsFor(tagId: Int): List<Int>
}