package com.example.notes.ui.recycler

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.example.notes.R
import com.example.notes.models.Note
import java.util.*

class NotesAdapter(
    context: Context
): RecyclerView.Adapter<NotesAdapter.ViewHolder>() {

    var onItemClick: ((Note) -> Unit)? = null
    var onDeleteButtonClick: ((Note) -> Unit)? = null

    private val inflater: LayoutInflater = LayoutInflater.from(context)
    private var notes = mutableListOf<Note>()

    inner class ViewHolder(itemView: View): RecyclerView.ViewHolder(itemView) {
        val title: TextView = itemView.findViewById(R.id.noteCellTitle)
        val content: TextView = itemView.findViewById(R.id.noteCellContent)

        init {
            itemView.setOnClickListener {
                onItemClick?.invoke(notes[adapterPosition])
            }
            val deleteButton: ImageButton = itemView.findViewById(R.id.deleteNote)
            deleteButton.setOnClickListener {
                onDeleteButtonClick?.invoke(notes[adapterPosition])
//                notes.remove(notes[adapterPosition])
            }
        }
    }

    fun setupNotes(notes: MutableList<Note>) {
        this.notes = notes
        notifyDataSetChanged()
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val cell = inflater.inflate(R.layout.note_list_cell, parent,false)
        return ViewHolder(cell)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val note = notes[position]
        holder.title.text = note.title
        holder.content.text = note.content ?: note.lastUpdate.toString()
    }

    override fun getItemCount(): Int = notes.count()
}