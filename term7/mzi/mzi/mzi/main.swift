//
//  main.swift
//  mzi
//
//  Created by Kirill Gorbachyonok on 9/13/20.
//

import Foundation

guard let strings = readFromFile(named: "text", withExtension: "txt") else { exit(-9) }


let string = strings[0]
let key = strings[1]

let encodedString = DES.encode(this: string, using: key)
print("ENCODED: ", encodedString)
print("DECODED: ", DES.decode(this: encodedString, using: key))

