package com.example.notes.interfaces

import androidx.lifecycle.LiveData
import androidx.room.*
import com.example.notes.models.Note
import com.example.notes.models.Tag

@Dao
interface TagsDao {
    @Insert
    suspend fun insert(tag: Tag): Long

    @Query("SELECT * FROM tags")
    suspend fun getAll(): List<Tag>

    @Update
    fun update(tag: Tag)

    @Delete
    fun delete(tag: Tag)

    @Query("DELETE FROM tags")
    suspend fun deleteAll()

    @Query("SELECT * FROM tags WHERE id = :id")
    suspend fun findBy(id: Int): Tag?

    @Query("SELECT id FROM tags WHERE name = :name")
    suspend fun findIdBy(name: String): Int
}