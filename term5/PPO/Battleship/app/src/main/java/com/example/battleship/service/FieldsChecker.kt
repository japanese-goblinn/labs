package com.example.battleship.service

class FieldsChecker {
    companion object {
        fun check(text: String?): Boolean {
            if (text.isNullOrEmpty()) {
                return false
            } else {
                return true
            }
        }
    }
}