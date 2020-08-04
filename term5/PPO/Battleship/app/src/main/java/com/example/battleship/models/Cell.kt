package com.example.battleship.models

import android.graphics.Rect

enum class CellType {
    WATER,
    HIT,
    MISS,
    SHIP,
    DESTROYED
}

class Cell {
    var type = CellType.WATER
    var rect = Rect()

    fun contains(point: Pair<Int, Int>): Boolean = rect.contains(point.first, point.second)

}