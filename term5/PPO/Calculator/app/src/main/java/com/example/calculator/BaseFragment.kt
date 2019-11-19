package com.example.calculator

import android.content.Context
import android.content.res.Configuration
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment


class BaseFragment: Fragment() {

    private lateinit var button0: Button
    private lateinit var buttonDot: Button
    private lateinit var buttonComma: Button
    private lateinit var button1: Button
    private lateinit var button2: Button
    private lateinit var button3: Button
    private lateinit var buttonPlus: Button
    private lateinit var button4: Button
    private lateinit var button5: Button
    private lateinit var button6: Button
    private lateinit var buttonMinus: Button
    private lateinit var button7: Button
    private lateinit var button8: Button
    private lateinit var button9: Button
    private lateinit var buttonMul: Button
    private lateinit var buttonClear: Button
    private lateinit var buttonPow: Button
    private lateinit var buttonDiv: Button
    private lateinit var changeModeButton: Button

    var delegate: Calculatable? = null

    private var isScientificMode = false

    override fun onAttach(context: Context) {
        super.onAttach(context)
        delegate = activity as? MainActivity
    }

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_base, container, false)
        setupLayoutFor(view)
        return view
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        changeModeButtonState()
    }

    private fun setupLayoutFor(view: View) {
        button0 = view.findViewById(R.id.button0)
        button0.setOnClickListener {
            delegate?.push(getString(R.string.button0))
        }

        buttonDot = view.findViewById(R.id.button_dot)
        buttonDot.setOnClickListener {
            delegate?.push(getString(R.string.dot))
        }

        buttonComma = view.findViewById(R.id.button_comma)
        buttonComma.setOnClickListener {
            delegate?.push(getString(R.string.comma))
        }

        button1 = view.findViewById(R.id.button1)
        button1.setOnClickListener {
            delegate?.push(getString(R.string.button1))
        }

        button2 = view.findViewById(R.id.button2)
        button2.setOnClickListener {
            delegate?.push(getString(R.string.button2))
        }

        button3 = view.findViewById(R.id.button3)
        button3.setOnClickListener {
            delegate?.push(getString(R.string.button3))
        }

        buttonPlus = view.findViewById(R.id.button_plus)
        buttonPlus.setOnClickListener {
            delegate?.push(getString(R.string.plus))
        }

        button4 = view.findViewById(R.id.button4)
        button4.setOnClickListener {
            delegate?.push(getString(R.string.button4))
        }

        button5 = view.findViewById(R.id.button5)
        button5.setOnClickListener {
            delegate?.push(getString(R.string.button5))
        }

        button6 = view.findViewById(R.id.button6)
        button6.setOnClickListener {
            delegate?.push(getString(R.string.button6))
        }

        buttonMinus = view.findViewById(R.id.button_minus)
        buttonMinus.setOnClickListener {
            delegate?.push(getString(R.string.minus))
        }

        button7 = view.findViewById(R.id.button7)
        button7.setOnClickListener {
            delegate?.push(getString(R.string.button7))
        }

        button8 = view.findViewById(R.id.button8)
        button8.setOnClickListener {
            delegate?.push(getString(R.string.button8))
        }

        button9 = view.findViewById(R.id.button9)
        button9.setOnClickListener {
            delegate?.push(getString(R.string.button9))
        }

        buttonMul = view.findViewById(R.id.button_mul)
        buttonMul.setOnClickListener {
            delegate?.push("*")
        }

        buttonClear = view.findViewById(R.id.button_clear)
        buttonClear.setOnClickListener {
            delegate?.pop()
        }

        buttonClear.setOnLongClickListener {
            delegate?.clearAll()
            return@setOnLongClickListener true
        }

        buttonPow = view.findViewById(R.id.button_pow)
        buttonPow.setOnClickListener {
            delegate?.push("^")
        }

        buttonDiv = view.findViewById(R.id.button_div)
        buttonDiv.setOnClickListener {
            delegate?.push("/")
        }

        changeModeButton = view.findViewById(R.id.button_mode)
        changeModeButton.setOnClickListener {
            toggleMode()
        }
    }

    private fun changeModeButtonState() {
        val orientation = resources.configuration.orientation
        if (orientation == Configuration.ORIENTATION_LANDSCAPE) {
            changeModeButton.isEnabled = false
            changeModeButton.setBackgroundResource(R.drawable.rounder_dimmed_mode_button)
            isScientificMode = true
        } else {
            changeModeButton.isEnabled = true
            changeModeButton.setBackgroundResource(R.drawable.rounded_mode_button)
        }
        if (BuildConfig.FLAVOR.equals("demo") || BuildConfig.FLAVOR.equals("demoDebug")) {
            changeModeButton.isEnabled = true
            changeModeButton.setBackgroundResource(R.drawable.rounder_dimmed_mode_button)
            isScientificMode = false
            changeModeButton.setOnClickListener {
                Toast.makeText(
                    activity, "Nope, buy full version", Toast.LENGTH_LONG
                )
                .show()
            }
        }
        checkMode()
    }

    private fun toggleMode() {
        isScientificMode = !isScientificMode
        checkMode()
    }

    private fun checkMode() {
        fragmentManager?.let {
            val scientificFragment = it.findFragmentById(R.id.scientific_fragment)
            scientificFragment?.let {
                val view = it.view
                if (isScientificMode) {
                    view?.visibility = View.VISIBLE
                    changeModeButton.text = getString(R.string.base_mode)
                } else {
                    view?.visibility = View.GONE
                    changeModeButton.text = getString(R.string.scientific_mode)
                }
            }
        }
    }
}