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
import java.util.concurrent.Executor
import java.util.concurrent.Executors

@Database(
    entities = [Note::class, Tag::class, NoteTag::class],
    version = 2
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
//            .addCallback(NotesDatabaseCallback(scope))
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
            notesDao.deleteAll()
            tagsDao.deleteAll()
            notesTagsDao.deleteAll()
            notesDao.insert(Note(title =  "Really long title like you can see booo or somethins", content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."))
            notesDao.insert(Note(title = "Boop 1", content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."))
            notesDao.insert(Note(title = "No gray", content = null))
        }
    }
}