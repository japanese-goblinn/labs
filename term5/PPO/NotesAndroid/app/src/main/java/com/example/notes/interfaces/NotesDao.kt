package com.example.notes.interfaces

import androidx.lifecycle.LiveData
import androidx.room.*
import com.example.notes.models.Note

@Dao
interface NotesDao {

    @Insert
    suspend fun insert(note: Note)

    @Query("SELECT * FROM notes")
    fun getAll(): LiveData<List<Note>>

    @Update
    fun update(note: Note)

    @Delete
    fun delete(note: Note)

    @Query("DELETE FROM notes")
    suspend fun deleteAll()
}