package com.example.myapplication.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.webkit.WebChromeClient
import android.webkit.WebView
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.navArgs
import com.example.myapplication.R

class ArticleFragment: Fragment() {

    private val args: ArticleFragmentArgs by navArgs()

    private lateinit var webView: WebView

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val rootView = inflater.inflate(R.layout.fragment_artical, container,false)
        setupLayoutFor(rootView)
        return rootView
    }

    private fun setupLayoutFor(view: View) {
        webView = view.findViewById(R.id.articalWebView)
        webView.settings.loadWithOverviewMode = true

        webView.settings.javaScriptEnabled = true
        webView.isHorizontalScrollBarEnabled = false
        webView.webChromeClient = WebChromeClient()

        webView.loadDataWithBaseURL(
            null,
            "<style>img{display: inline; height: auto; max-width: 100%;} " +
                "</style>\n" + "<style>iframe{ height: auto; width: auto;}" + "</style>\n"
                + args.articleContent, null, "utf-8", null
        )
    }
}