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

func readFromFile() -> [String]? {
    guard let fileURL = Bundle.main.url(forResource: "сaesar", withExtension: "txt"),
          let string = try? String(contentsOf: fileURL, encoding: .ascii)
        else { return nil }
    return string.components(separatedBy: .newlines)
}

let power = 26

func encode(this string: String, with key: Int) -> String {
    let encodeShift = { (char: Character) -> String in
        let charValue = Int(char.asciiValue!)
        let shift = char.shift
        var preResult = (charValue + key - shift) % power
        if preResult < 0 { preResult += power }
        return String(UnicodeScalar(preResult + shift)!)
    }
    return string.filter({ $0.isLetter }).map(encodeShift).reduce("", +)
}

func decode(this string: String, with key: Int) -> String {
    let decodeShift = { (char: Character) -> String in
        let charValue = Int(char.asciiValue!)
        let shift = char.shift
        var preResult = (charValue - key + power - shift) % power
        if preResult < 0 { preResult += power }
        return String(UnicodeScalar(preResult + shift)!)
    }
    return string.filter({ $0.isLetter }).map(decodeShift).reduce("", +)
}

guard let inputData = readFromFile(), let key = Int(inputData[0]) else { fatalError("FILE READ ERROR") }
let encodeString = inputData[1]
print("STRING TO ENCODE: ", encodeString)
print("KEY: ", key)

let encodedString = encode(this: encodeString, with: key)
print("ENCODED: ", encodedString)
print("DECODED: ", decode(this: encodedString, with: key))
