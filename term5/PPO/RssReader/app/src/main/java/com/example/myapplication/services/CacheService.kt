package com.example.myapplication.services

import android.app.Activity
import android.content.Context
import com.example.myapplication.data.Post
import com.squareup.moshi.JsonAdapter
import com.squareup.moshi.Moshi
import com.squareup.moshi.Types

class CacheService {

    companion object {

        private const val ARTICLES_CACHE = "ARTICLES_CACHE"
        private const val CONTEXT_NAME = "Cache"

        private val adapter: JsonAdapter<List<Post>>
            get() {
                val serializer = Moshi.Builder().build()
                val type = Types.newParameterizedType(
                    List::class.java, Post::class.java
                )
                return serializer.adapter(type)
            }

        private fun encode(
            adapter: JsonAdapter<List<Post>>,
            data: List<Post>
        ) = adapter.toJson(data)

        private fun decode(
            adapter: JsonAdapter<List<Post>>,
            data: String
        ) = adapter.fromJson(data)

        fun save(context: Context, data: List<Post>) {
            val sharedPreferences = context
                    .getSharedPreferences(CONTEXT_NAME, Context.MODE_PRIVATE)
            val jsonData = encode(adapter, data)
            val editor = sharedPreferences.edit()
            editor.putString(ARTICLES_CACHE ,jsonData)
            editor.apply()
        }

        fun load(context: Context): List<Post> {
            val sharedPreferences = context
                .getSharedPreferences(CONTEXT_NAME, Context.MODE_PRIVATE)
            val jsonData = sharedPreferences.getString(ARTICLES_CACHE, null)
            return jsonData?.let {
                decode(adapter, it)
            } ?: listOf()
        }
    }
}