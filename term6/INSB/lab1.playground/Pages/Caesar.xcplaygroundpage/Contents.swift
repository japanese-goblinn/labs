import Foundation

extension Character {
    var getShift: Int {
        if self.isUppercase {
            return Int(UnicodeScalar("A").value)
        } else {
            return Int(UnicodeScalar("a").value)
        }
    }
}

func readFromFile() -> String? {
    guard let fileURL = Bundle.main.url(forResource: "сaesar", withExtension: "txt")
        else { return nil }
    return try? String(contentsOf: fileURL, encoding: .ascii)
}

let power = 26

func encode(this string: String, with key: Int) -> String {
        
    let encodeShift = { (char: Character) -> Character in
        let charValue = Int(char.asciiValue!)
        let shift = char.getShift
        var preResult = (charValue + key - shift) % power
        if preResult < 0 { preResult += power }
        return Character(UnicodeScalar(preResult + shift)!)
    }
    
    return String(string.filter({ $0.isLetter }).map(encodeShift))
}

func decode(this string: String, with key: Int) -> String {
    
    let decodeShift = { (char: Character) -> Character in
        let charValue = Int(char.asciiValue!)
        let shift = char.getShift
        var preResult = (charValue - key + power - shift) % power
        if preResult < 0 { preResult += power }
        return Character(UnicodeScalar(preResult + shift)!)
    }
    
    return String(string.filter({ $0.isLetter }).map(decodeShift))
}

guard let encodeString = readFromFile() else { fatalError("FILE READ ERROR") }

let key = -1
let encodedString = encode(this: encodeString, with: key)
print(encodedString)
print(decode(this: encodedString, with: key))

//: [Next](@next)
