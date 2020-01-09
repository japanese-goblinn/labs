package com.example.battleship

import android.content.Context
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Rect
import android.util.AttributeSet
import android.view.View
import androidx.core.content.ContextCompat
import com.example.battleship.models.Cell
import com.example.battleship.models.CellType
import com.example.battleship.models.Direction
import com.example.battleship.models.Ship

class BattlefieldView: View {

    private val COUNT = 10

    private val paint = Paint()
    private val shipPaint = Paint()
    private val hitPaint = Paint()
    private val missPaint = Paint()
    private val destroyPaint = Paint()

    var cells = Array(COUNT) { Array(COUNT) { Cell() } }

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        init()
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        drawGrid(canvas)
        drawCells(canvas)
    }

    private fun init() {

        paint.color = ContextCompat.getColor(context, R.color.colorSecondary)
        paint.isAntiAlias = true
        paint.style = Paint.Style.STROKE
        paint.strokeWidth = resources.displayMetrics.density * 2

        shipPaint.color = ContextCompat.getColor(context, R.color.colorAccent)
        shipPaint.isAntiAlias = true
        shipPaint.style = Paint.Style.STROKE
        shipPaint.strokeWidth = resources.displayMetrics.density * 3

        hitPaint.color = ContextCompat.getColor(context, R.color.colorNegative)
        hitPaint.isAntiAlias = true
        hitPaint.style = Paint.Style.STROKE
        hitPaint.strokeWidth = resources.displayMetrics.density * 4

        destroyPaint.color = ContextCompat.getColor(context, R.color.colorNegative)
        destroyPaint.isAntiAlias = true
        destroyPaint.style = Paint.Style.FILL_AND_STROKE
        destroyPaint.strokeWidth = resources.displayMetrics.density

        missPaint.color = ContextCompat.getColor(context, R.color.colorSecondary)
        missPaint.isAntiAlias = true
        missPaint.style = Paint.Style.FILL_AND_STROKE
        missPaint.strokeWidth = resources.displayMetrics.density

        initCells()
    }

    private fun initCells() {
        val xUnit = (width * 0.1f).toInt()
        val yUnit = (height * 0.1f).toInt()
        for (i in 0 until COUNT) {
            for (j in 0 until  COUNT) {
                cells[i][j].rect = Rect(
                    j * xUnit,
                    i * yUnit,
                    (j + 1) * xUnit,
                    (i + 1) * yUnit
                )
            }
        }
    }

    fun getRectIndexesFor(x: Float, y: Float): Pair<Int, Int> {
        cells.forEachIndexed { i, cells ->
            for ((j, cell) in cells.withIndex()) {
                if (cell.contains(Pair(x.toInt(), y.toInt()))) {
                    return Pair(i, j)
                }
            }
        }
        return Pair(-1, -1)
    }

    fun drawHit(i: Int, j: Int) {
        cells[i][j].type = CellType.HIT
        invalidate()
    }

    fun drawMiss(i: Int, j: Int) {
        cells[i][j].type = CellType.MISS
        invalidate()
    }

    fun draw(ship: Ship, canDraw: ((Boolean) -> Unit)? = null) {
        var validationCheck = mutableListOf<Boolean>()
        when (ship.direction) {
            Direction.HORIZONTAL -> {
                val i = ship.i
                val start = ship.j
                val end = start + ship.rank.size - 1

                if (!isValidPlace(Direction.HORIZONTAL, i, end)) {
                    canDraw?.let { it(false) }
                    return
                }

                validationCheck.add(isAllWater(Direction.HORIZONTAL, i, start, end))
                validationCheck.add(isSurroundedByWater(ship))

                if (validationCheck.all { it == true }) {
                    if (isDestoyed(Direction.HORIZONTAL, i, start, end)) {
                        for (j in start..end) {
                            cells[i][j].type = CellType.DESTROYED
                        }
                    } else {
                        for (j in start..end) {
                            cells[i][j].type = CellType.SHIP
                        }
                    }
                    canDraw?.let { it(true) }
                } else {
                    canDraw?.let { it(false) }
                    return
                }
            }
            Direction.VERTICAL -> {
                val j = ship.j
                val start = ship.i
                val end = start + ship.rank.size - 1

                if (!isValidPlace(Direction.VERTICAL, j, end)) {
                    canDraw?.let { it(false) }
                    return
                }

                validationCheck.add(isAllWater(Direction.VERTICAL, j, start, end))
                validationCheck.add(isSurroundedByWater(ship))

                if (validationCheck.all { it == true }) {

                    if (isDestoyed(Direction.VERTICAL, j, start, end)) {
                        for (i in start..end) {
                            cells[i][j].type = CellType.DESTROYED
                        }
                    } else {
                        for (i in start..end) {
                            cells[i][j].type = CellType.SHIP
                        }
                    }
                    canDraw?.let { it(true) }
                } else {
                    canDraw?.let { it(false) }
                    return
                }
            }
        }
        invalidate()
    }

    private fun isDestoyed(direction: Direction, indexFrom: Int, start: Int, end: Int): Boolean {
        when (direction) {
            Direction.HORIZONTAL -> {
                for (j in start..end) {
                    if (cells[indexFrom][j].type != CellType.HIT) {
                        return false
                    }
                }
            }
            Direction.VERTICAL -> {
                for (i in start..end) {
                    if (cells[i][indexFrom].type != CellType.HIT) {
                        return false
                    }
                }
            }
        }
        return true
    }

    private fun isSurroundedByWater(ship: Ship): Boolean {
        when (ship.direction) {
            Direction.VERTICAL -> {
                val j = ship.j
                val start = ship.i
                val end = start + ship.rank.size - 1

                cells.getOrNull(start - 1)?.get(j)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(end + 1)?.get(j)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(start - 1)?.getOrNull(j - 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(end + 1)?.getOrNull(j - 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(start - 1)?.getOrNull(j + 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(end + 1)?.getOrNull(j + 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                for (i in start..end) {
                    cells[i].getOrNull(j - 1)?.let {
                        if (isNot(CellType.WATER, it)) {
                            return false
                        }
                    }
                    cells[i].getOrNull(j + 1)?.let {
                        if (isNot(CellType.WATER, it)) {
                            return false
                        }
                    }
                }

            }
            Direction.HORIZONTAL -> {
                val i = ship.i
                val start = ship.j
                val end = start + ship.rank.size - 1

                cells[i].getOrNull(start - 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells[i].getOrNull(end + 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(i - 1)?.getOrNull(start - 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(i - 1)?.getOrNull(end + 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(i + 1)?.getOrNull(start - 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                cells.getOrNull(i + 1)?.getOrNull(end + 1)?.let {
                    if (isNot(CellType.WATER, it)) {
                        return false
                    }
                }

                for (j in start..end) {
                    cells.getOrNull(i - 1)?.get(j)?.let {
                        if (isNot(CellType.WATER, it)) {
                            return false
                        }
                    }
                    cells.getOrNull(i + 1)?.get(j)?.let {
                        if (isNot(CellType.WATER, it)) {
                            return false
                        }
                    }
                }
            }
        }
        return true
    }

    private fun isNot(type: CellType, cell: Cell) = when (type) {

        CellType.WATER -> cell.type != CellType.WATER

        else -> false
    }

    private fun isAllWater(direction: Direction, indexFrom: Int, start: Int, end: Int): Boolean {
        when (direction) {
            Direction.HORIZONTAL -> {
                for (j in start..end) {
                    if (cells[indexFrom][j].type != CellType.WATER) {
                        return false
                    }
                }
            }
            Direction.VERTICAL -> {
                for (i in start..end) {
                    if (cells[i][indexFrom].type != CellType.WATER) {
                        return false
                    }
                }
            }
        }
        return true
    }

    private fun isValidPlace(direction: Direction, indexFrom: Int, end: Int): Boolean {
        when (direction) {
            Direction.HORIZONTAL -> {
                cells.getOrNull(indexFrom)?.getOrNull(end)?.let {
                    return true
                } ?: return false
            }
            Direction.VERTICAL -> {
                cells.getOrNull(end)?.getOrNull(indexFrom)?.let {
                    return true
                } ?: return false
            }
        }
    }

    private fun drawCells(canvas: Canvas) {
        for (cellRow in cells) {
            for (cell in cellRow) {
                when (cell.type) {
                    CellType.SHIP -> {
                        canvas.drawRect(cell.rect, shipPaint)
                    }
                    CellType.MISS -> {
                        canvas.drawRect(cell.rect, missPaint)
                    }
                    CellType.HIT -> {
                        canvas.drawRect(cell.rect, hitPaint)
                    }
                    CellType.DESTROYED -> {
                        canvas.drawRect(cell.rect, destroyPaint)
                    }
                }
            }
        }
    }

    private fun drawGrid(canvas: Canvas) {
        drawHorizontalLines(canvas)
        drawVerticalLines(canvas)
    }

    private fun drawVerticalLines(canvas: Canvas) {
        var drawPartition = 0f
        while (drawPartition <= width) {
            canvas.drawLine(
                width * drawPartition,
                0f,
                width * drawPartition,
                height.toFloat(), paint
            )
            drawPartition += 0.1f
        }
    }

    private fun drawHorizontalLines(canvas: Canvas) {
        var drawPartition = 0f
        while (drawPartition <= height) {
            canvas.drawLine(
                0f,
                height * drawPartition,
                width.toFloat(),
                height * drawPartition, paint
            )
            drawPartition += 0.1f
        }
    }
}