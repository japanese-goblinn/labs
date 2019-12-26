package com.example.myapplication.viewModels

import android.app.Application
import android.content.Context
import android.net.ConnectivityManager
import android.util.Log
import android.widget.Toast
import androidx.core.content.ContextCompat.getSystemService
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import com.example.myapplication.data.Post
import com.example.myapplication.services.CacheService
import com.example.myapplication.services.ParserService
import kotlinx.coroutines.launch

class FeedViewModel(val app: Application): AndroidViewModel(app) {

    var url: String = "https://www.androidauthority.com/feed"
    var posts = MutableLiveData<List<Post>>()

    init {
        getPosts()
    }

    private val isNetworkAvailable: Boolean
        get() {
            val manager = app
                .getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager
            val networkInfo = manager.activeNetworkInfo
            return networkInfo?.let { it.isConnectedOrConnecting } ?: false
        }

    fun getPosts() = viewModelScope.launch {
        if (isNetworkAvailable) {
            posts.value = ParserService.feedFor(url).also {
                if (it.isEmpty()) {
                    Toast
                        .makeText(app, "This RSS link is not valid", Toast.LENGTH_LONG)
                        .show()
                } else {
                    CacheService.save(app, it.takeLast(10))
                }
            }
        } else {
            val cachePosts = CacheService.load(app)
            posts.value = cachePosts
        }
    }

}