package com.example.notes.db

import android.content.Context
import android.util.Log
import androidx.room.*
import androidx.room.Database
import androidx.sqlite.db.SupportSQLiteDatabase
import com.example.notes.interfaces.NotesDao
import com.example.notes.interfaces.NotesTagsDao
import com.example.notes.interfaces.TagsDao
import com.example.notes.models.Note
import com.example.notes.models.NoteTag
import com.example.notes.models.Tag
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import java.util.concurrent.Executor
import java.util.concurrent.Executors

@Database(
    entities = [Note::class, Tag::class, NoteTag::class],
    version = 4
)
@TypeConverters(Converters::class)
abstract class Database: RoomDatabase() {

    abstract fun notesDao(): NotesDao

    abstract fun tagsDao(): TagsDao

    abstract fun notesTagsDao(): NotesTagsDao

    companion object {

        @Volatile
        private var INSTANCE: com.example.notes.db.Database? = null

        fun get(context: Context, scope: CoroutineScope): com.example.notes.db.Database {
            return INSTANCE ?: synchronized(this) {
                newInstance(context, scope)
            }
        }

        private fun newInstance(context: Context, scope: CoroutineScope): com.example.notes.db.Database {
            val instance = Room.databaseBuilder(
                context.applicationContext,
                com.example.notes.db.Database::class.java,
                "notes"
            )
            .fallbackToDestructiveMigration()
            .addCallback(NotesDatabaseCallback(scope))
            .build()
            INSTANCE = instance
            return instance
        }

    }

    private class NotesDatabaseCallback(private val scope: CoroutineScope): Callback() {
        override fun onOpen(db: SupportSQLiteDatabase) {
            super.onOpen(db)
            INSTANCE?.let { database ->
                scope.launch {
                    populateDatabase(
                        database.notesDao(),
                        database.tagsDao(),
                        database.notesTagsDao()
                    )
                }
            }
        }

        suspend fun populateDatabase(
            notesDao: NotesDao,
            tagsDao: TagsDao,
            notesTagsDao: NotesTagsDao
        ) {
            notesTagsDao.deleteAll()
            notesDao.deleteAll()
            tagsDao.deleteAll()

            val tagId = tagsDao.insert(Tag(name = "note"))
            val tagId2 = tagsDao.insert(Tag(name = "Cool"))
            val note = notesDao.insert(Note(title = "C", content = null)).toInt()
            notesTagsDao.insert(
                NoteTag(
                    noteId = note,
                    tagId = tagId.toInt()
                )
            )
            notesTagsDao.insert(
                NoteTag(
                    noteId = note,
                    tagId = tagId2.toInt()
                )
            )
            notesTagsDao.insert(
                NoteTag(
                    noteId = notesDao.insert(Note(title =  "C", content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")).toInt(),
                    tagId = tagId.toInt()
            ))
            notesDao.insert(Note(title = "B", content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."))
        }
    }
}