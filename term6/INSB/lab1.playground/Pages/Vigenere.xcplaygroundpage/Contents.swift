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
        var resultKey = (0..<repeatAmount).reduce(into: "") { res, _ in res += key }
        resultKey += key[key.startIndex..<key.index(key.startIndex, offsetBy: stopIndex)]
        return resultKey
    }
}

func readFromFile() -> [String]? {
    guard let fileURL = Bundle.main.url(forResource: "vigenre", withExtension: "txt")
        else { return nil }
    return (try? String(contentsOf: fileURL, encoding: .ascii))?.components(separatedBy: .newlines)
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

guard let inputData = readFromFile() else { fatalError("FILE READ ERROR") }
let key = inputData[0]
let encodeString = inputData[1].filter { $0.isLetter }
print("STRING TO ENCODE: ", encodeString)
print("KEY: ", key)

let encodedString = encode(this: encodeString, using: key)
print("ENCODED: ", encodedString)
print("DECODED: ", decode(this: encodedString, using: key))

