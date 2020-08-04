import Foundation

extension Character {
    var shift: Int {
        if self.isUppercase {
            return Int(UnicodeScalar("A").value)
        } else {
            return Int(UnicodeScalar("a").value)
        }
    }
}

extension String {
    func encode(using key: Self) -> String {
        let repeatAmount = self.count / key.count
        let stopIndex = self.count % key.count
        let resultKey = (0..<repeatAmount).reduce(into: "") { res, _ in res += key }
        return resultKey + key.prefix(stopIndex)
    }
}

let encodeShift = { (char: Character, key: Int) -> Character in
    let charValue = Int(char.asciiValue!)
    let shift = char.shift
    var preResult = (charValue + key - shift) % 26
    if preResult < 0 { preResult += 26 }
    return Character(UnicodeScalar(preResult + shift)!)
}

let decodeShift = { (char: Character, key: Int) -> Character in
    let charValue = Int(char.asciiValue!)
    let shift = char.shift
    var preResult = (charValue - key + 26 - shift) % 26
    if preResult < 0 { preResult += 26 }
    return Character(UnicodeScalar(preResult + shift)!)
}

let lowerAlphabet = (UnicodeScalar("a").value...UnicodeScalar("z").value)
    .compactMap { UnicodeScalar($0) }
    .map { Character($0) }

let lowerTable = lowerAlphabet
    .enumerated()
    .reduce(into: [Character: [Character]]()) { (dict, pair) in
        let (i, c) = pair
        dict[c] = lowerAlphabet.map { char in encodeShift(char, i) }
    }

let upperAlphabet = (UnicodeScalar("A").value...UnicodeScalar("Z").value)
    .compactMap { UnicodeScalar($0) }
    .map { Character($0) }

let upperTable = upperAlphabet
    .enumerated()
    .reduce(into: [Character: [Character]]()) { (dict, pair) in
        let (i, c) = pair
        dict[c] = upperAlphabet.map { char in encodeShift(char, i) }
    }

func encode(this string: String, using key: String) -> String {
    return zip(string, string.encode(using: key))
        .map { (encodeChar: Character, keyChar: Character) -> String in
            let index = Int(keyChar.asciiValue!) - keyChar.shift
            if encodeChar.isUppercase {
                return String(upperTable[encodeChar]![index])
            } else {
                return String(lowerTable[encodeChar]![index])
            }
        }
        .reduce("", +)
}

func decode(this string: String, using key: String) -> String {
    return zip(string, string.encode(using: key))
        .map { (decodeChar: Character, keyChar: Character) -> String in
            let index = Int(keyChar.asciiValue!) - keyChar.shift
            return String(decodeShift(decodeChar, index))
        }
        .reduce("", +)
}

let key = "LEMON"
let string = "ATTACKATDAWN"

let encodedString = encode(this: string, using: key)
print("ENCODED: ", encodedString)
print("DECODED: ", decode(this: encodedString, using: key))

