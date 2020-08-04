package com.example.myapplication.ui

import android.app.AlertDialog
import android.content.DialogInterface
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ProgressBar
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProviders
import androidx.navigation.fragment.findNavController
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.myapplication.R
import com.example.myapplication.recycler.FeedAdapter
import com.example.myapplication.viewModels.FeedViewModel
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.textfield.TextInputEditText

class FeedFragment: Fragment() {

    private lateinit var postsList: RecyclerView
    private lateinit var viewModel: FeedViewModel
    private lateinit var viewAdapter: FeedAdapter

    private lateinit var progressBar: ProgressBar
    private lateinit var fab: FloatingActionButton

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_feed, container,false)
        setupLayoutFor(view)
        return view
    }

    private fun setupLayoutFor(rootView: View) {

        viewModel = ViewModelProviders
            .of(this)
            .get(FeedViewModel::class.java)

        postsList = rootView.findViewById(R.id.postsList)

        viewAdapter = FeedAdapter(requireContext())

        postsList.adapter = viewAdapter

        postsList.layoutManager = LinearLayoutManager(requireContext())
        val decoration = DividerItemDecoration(
            requireContext(),
            DividerItemDecoration.VERTICAL
        )
        decoration.setDrawable(
            ContextCompat.getDrawable(requireContext(), R.drawable.divider)!!
        )
        postsList.addItemDecoration(decoration)

        viewAdapter.onItemClick = {
            val action = FeedFragmentDirections
                .actionFeedFragmentToArticalFragment(it.content, it.title)
            findNavController().navigate(action)
        }

        progressBar = rootView.findViewById(R.id.progressBar)

        fab = rootView.findViewById(R.id.fab)
        fab.setOnClickListener {
            val builder = androidx.appcompat.app.AlertDialog.Builder(requireContext(), R.style.dialog)
            val view = layoutInflater.inflate(R.layout.dialog_link, null)
            val urlInput: TextInputEditText = view.findViewById(R.id.urlTextInputEditText)
            builder.setView(view)
            builder.setPositiveButton("Use") { _, _ ->
                viewModel.url = urlInput.text.toString()
                progressBar.visibility = View.VISIBLE
                viewModel.getPosts()
            }
            builder.setNegativeButton("Cancel") { dialog, _ ->
                dialog.cancel()
            }
            val dialog = builder.create()
            dialog.show()
        }

        viewModel.posts.observe(
            viewLifecycleOwner,
            Observer {
                viewAdapter.setupPosts(it)
                progressBar.visibility = View.GONE
            }
        )
    }
}