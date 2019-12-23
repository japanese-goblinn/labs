package com.example.notes.interfaces

import androidx.lifecycle.LiveData
import androidx.room.*
import com.example.notes.models.Note
import com.example.notes.models.Tag

@Dao
interface TagsDao {
    @Insert
    suspend fun insert(tag: Tag)

    @Query("SELECT * FROM tags")
    fun getAll(): LiveData<List<Tag>>

    @Update
    fun update(tag: Tag)

    @Delete
    fun delete(tag: Tag)
}