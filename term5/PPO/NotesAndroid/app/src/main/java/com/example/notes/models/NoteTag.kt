package com.example.notes.models

import androidx.room.*
import androidx.room.ForeignKey.CASCADE

@Entity(
    tableName = "notes_tags",
    indices = [
        Index("note_id"),
        Index("tag_id")
    ],
    foreignKeys = [
        ForeignKey(
            entity = Note::class,
            parentColumns = ["id"],
            childColumns = ["note_id"],
            onDelete = CASCADE
        ),
        ForeignKey(
            entity = Tag::class,
            parentColumns = ["id"],
            childColumns = ["tag_id"],
            onDelete = CASCADE
        )
    ]
)
data class NoteTag(
    @PrimaryKey(autoGenerate = true) val id: Int = 0,
    @ColumnInfo(name = "note_id") val noteId: Int,
    @ColumnInfo(name = "tag_id") val tagId: Int
)