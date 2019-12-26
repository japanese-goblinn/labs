package com.example.myapplication.services

import com.example.myapplication.data.Post
import com.prof.rssparser.Parser
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

class ParserService {

    companion object {

        suspend fun feedFor(url: String): List<Post> = withContext(Dispatchers.Main) {
            val parser = Parser()
            try {
                return@withContext parser.getArticles(url).map {
                    Post(
                        title = it.title ?: "",
                        publicationDate = it.pubDate ?: "",
                        previewContent = it.description ?: "",
                        content = it.content ?: "",
                        image = it.image ?: ""
                    )
                }
            } catch (e: Exception) {
                return@withContext listOf<Post>()
            }
        }
    }
}
