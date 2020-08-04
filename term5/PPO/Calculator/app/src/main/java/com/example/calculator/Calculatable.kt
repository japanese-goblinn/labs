package com.example.calculator

import javax.xml.xpath.XPathExpression

interface Calculatable {

    fun push(element: String)

    fun pop()

    fun clearAll()

}