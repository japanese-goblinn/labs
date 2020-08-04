package com.example.battleship.models

data class Hit(
    val type: CellType = CellType.WATER,
    val i: Int = -1,
    val j: Int = -1
)