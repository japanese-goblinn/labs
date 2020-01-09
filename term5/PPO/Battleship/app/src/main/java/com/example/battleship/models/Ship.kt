package com.example.battleship.models

enum class Direction {
    HORIZONTAL,
    VERTICAL
}

enum class Rank(val size: Int) {
    AIRCRAFT_CARRIER(5),
    BATTLESHIP(4),
    CRUISER(3),
    DESTROYER(2),
    SUBMARINE(1)
}

data class Ship(
    val rank: Rank = Rank.AIRCRAFT_CARRIER,
    val direction: Direction = Direction.VERTICAL,
    val i: Int = -1,
    val j: Int = -1
)

