package com.example.calculator

import android.util.Log
import org.mariuszgromada.math.mxparser.Expression

class Calculator {

    companion object {

        fun calculate(expression: String): String
                = dropMantissaIfZero("${Expression(expression).calculate()}")

        private fun dropMantissaIfZero(expression: String): String {
            if (expression.endsWith(".0")) {
                return expression.split(".").first()
            }
            return expression
        }
    }
}