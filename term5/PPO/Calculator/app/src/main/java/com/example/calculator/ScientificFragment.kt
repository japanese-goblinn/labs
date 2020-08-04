package com.example.calculator

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.Fragment

class ScientificFragment: Fragment() {

    private lateinit var buttonOpenParenthesis: Button
    private lateinit var buttonClosingParenthesis: Button
    private lateinit var buttonFact: Button
    private lateinit var buttonPercent: Button
    private lateinit var buttonSin: Button
    private lateinit var buttonCos: Button
    private lateinit var buttonTg: Button
    private lateinit var buttonCtg: Button
    private lateinit var buttonLn: Button
    private lateinit var buttonLog: Button
    private lateinit var buttonLog2: Button
    private lateinit var buttonLog10: Button
    private lateinit var buttonExponent: Button
    private lateinit var buttonPi: Button
    private lateinit var buttonAbs: Button
    private lateinit var buttonSqrt: Button

    private var delegate: Calculatable? = null

    override fun onAttach(context: Context) {
        super.onAttach(context)
        delegate = activity as? MainActivity
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_scientific, container, false)
        setupLayoutFor(view)
        return view
    }

    private fun setupLayoutFor(view: View) {
        buttonOpenParenthesis = view.findViewById(R.id.button_open_parenthesis)
        buttonOpenParenthesis.setOnClickListener {
            delegate?.push(getString(R.string.open_parenthesis))
        }

        buttonClosingParenthesis = view.findViewById(R.id.button_closing_parenthesis)
        buttonClosingParenthesis.setOnClickListener {
            delegate?.push(getString(R.string.closing_parenthesis))
        }

        buttonFact = view.findViewById(R.id.button_fact)
        buttonFact.setOnClickListener {
            delegate?.push(getString(R.string.factorial))
        }

        buttonPercent = view.findViewById(R.id.button_percent)
        buttonPercent.setOnClickListener {
            delegate?.push(getString(R.string.percent))
        }

        buttonSin = view.findViewById(R.id.button_sin)
        buttonSin.setOnClickListener {
            delegate?.push(getString(R.string.sin) + "(")
        }

        buttonCos = view.findViewById(R.id.button_cos)
        buttonCos.setOnClickListener {
            delegate?.push(getString(R.string.cos) + "(")
        }

        buttonTg = view.findViewById(R.id.button_tg)
        buttonTg.setOnClickListener {
            delegate?.push(getString(R.string.tg) + "(")
        }

        buttonCtg = view.findViewById(R.id.button_ctg)
        buttonCtg.setOnClickListener {
            delegate?.push(getString(R.string.ctg) + "(")
        }

        buttonLn = view.findViewById(R.id.button_ln)
        buttonLn.setOnClickListener {
            delegate?.push(getString(R.string.ln) + "(")
        }

        buttonLog = view.findViewById(R.id.button_log)
        buttonLog.setOnClickListener {
            delegate?.push(getString(R.string.log) + "(")
        }

        buttonLog2 = view.findViewById(R.id.button_log2)
        buttonLog2.setOnClickListener {
            delegate?.push(getString(R.string.log2) + "(")
        }

        buttonLog10 = view.findViewById(R.id.button_log10)
        buttonLog10.setOnClickListener {
            delegate?.push(getString(R.string.log10) + "(")
        }

        buttonExponent = view.findViewById(R.id.button_exp)
        buttonExponent.setOnClickListener {
            delegate?.push(getString(R.string.e))
        }

        buttonPi = view.findViewById(R.id.button_pi)
        buttonPi.setOnClickListener {
            delegate?.push("pi")
        }

        buttonAbs = view.findViewById(R.id.button_abs)
        buttonAbs.setOnClickListener {
            delegate?.push(getString(R.string.abs) + "(")
        }

        buttonSqrt = view.findViewById(R.id.button_sqrt)
        buttonSqrt.setOnClickListener {
            delegate?.push("sqrt(")
        }

    }
}