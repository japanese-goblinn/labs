package com.example.notes.models

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.ForeignKey
import androidx.room.PrimaryKey

@Entity(
    tableName = "notes_tags",
    foreignKeys = [
        ForeignKey(
            entity = Note::class,
            parentColumns = ["id"],
            childColumns = ["note_id"]
        ),
        ForeignKey(
            entity = Tag::class,
            parentColumns = ["id"],
            childColumns = ["tag_id"]
        )
    ]
)
data class NoteTag(
    @PrimaryKey(autoGenerate = true) val id: Int,
    @ColumnInfo(name = "note_id") val noteId: Int,
    @ColumnInfo(name = "tag_id") val tagId: Int
)