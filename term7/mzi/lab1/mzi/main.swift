//
//  main.swift
//  mzi
//
//  Created by Kirill Gorbachyonok on 9/13/20.
//

import Foundation

func getFileData() -> (String, String, String)? {
    guard let strings = readFromFile(named: "text", withExtension: "txt") else { return nil }
    return (strings[0], strings[1], strings[2])
}

func doubleDES() {
    print("\nDOUBLE DES")
    print(repeatElement("-", count: 10).reduce("", +))
    guard let (string, key1, key2) = getFileData() else { fatalError("Info read error") }
    let encodedString = DES.encode(this: DES.encode(this: string, using: key1), using: key2)
    print("ENCODED: ", encodedString)
    let decoded = DES.decode(this: DES.decode(this: encodedString, using: key2), using: key1)
    print("DECODED: ", decoded)
}

func tripleDES() {
    print("\nTRIPLE DES")
    print(repeatElement("-", count: 10).reduce("", +))
    guard let (string, key1, key2) = getFileData() else { fatalError("Info read error") }
    let encodedString = DES.encode(
        this: DES.decode(
            this: DES.encode(
                this: string,
                using: key1
            ),
            using: key2
        ),
        using: key1
    )
    print("ENCODED: ", encodedString)
    let decoded = DES.decode(
        this: DES.encode(
            this: DES.decode(
                this: encodedString,
                using: key1
            ),
            using: key2
        ),
        using: key1
    )
    print("DECODED: ", decoded)
}

func gost() {
    print("\nGOST 28147-89")
    print(repeatElement("-", count: 10).reduce("", +))
    guard let (string, key, _) = getFileData() else { fatalError("Info read error") }
    let encodedString = GOST.encode(this: string, using: key)
    print("ENCODED: ", encodedString)
    let decoded = GOST.decode(this: encodedString, using: key)
    print("DECODED: ", decoded)
}

doubleDES()
tripleDES()
gost()
