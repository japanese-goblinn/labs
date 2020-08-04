package com.example.myapplication.recycler

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.example.myapplication.R
import com.example.myapplication.data.Post
import com.squareup.picasso.Picasso
import kotlinx.android.synthetic.main.artical_cell.view.*
import java.text.ParseException
import java.text.SimpleDateFormat
import java.util.*

class FeedAdapter(
    context: Context
): RecyclerView.Adapter<FeedAdapter.ViewHolder>() {

    inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val imageView: ImageView = itemView.findViewById(R.id.articalImageView)
        val titleTextView: TextView = itemView.findViewById(R.id.titleTextView)
        val pubDateTextView: TextView = itemView.findViewById(R.id.publishDateTextView)
        val previewTextView: TextView = itemView.findViewById(R.id.contentPreviewTextView)

        init {
            itemView.setOnClickListener {
                onItemClick?.invoke(posts[adapterPosition])
            }
        }
    }

    var onItemClick: ((Post) -> Unit)? = null

    private val inflater: LayoutInflater = LayoutInflater.from(context)

    private var posts = mutableListOf<Post>()

    fun setupPosts(posts: List<Post>) {
        this.posts = posts.toMutableList()
        notifyDataSetChanged()
    }

    override fun getItemCount(): Int = posts.count()

    override fun onCreateViewHolder(
        parent: ViewGroup,
        viewType: Int
    ) = ViewHolder(inflater.inflate(R.layout.artical_cell, parent,false))


    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val post = posts[position]
        holder.titleTextView.text = post.title
        holder.previewTextView.text = post.previewContent

        holder.pubDateTextView.text = try {
            val sourceDateString = post.publicationDate

            val sourceSdf = SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss Z", Locale.ENGLISH)
            val date = sourceSdf.parse(sourceDateString)

            val sdf = SimpleDateFormat("dd MMMM yyyy", Locale.getDefault())
            sdf.format(date)

        } catch (e: ParseException) {
            e.printStackTrace()
            post.publicationDate
        }

        if (post.image.trim().count() == 0) {
            holder.imageView.setImageResource(R.drawable.placeholder)
        } else {
            Picasso
                .get()
                .load(post.image)
                .into(holder.imageView)
        }
    }
}