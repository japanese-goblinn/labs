package com.example.myapplication.data

import android.os.Parcelable
import kotlinx.android.parcel.Parcelize

@Parcelize
data class Post(
    val title: String,
    val publicationDate: String,
    val previewContent: String,
    val content: String,
    val image: String
): Parcelable