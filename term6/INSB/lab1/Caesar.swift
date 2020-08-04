import Foundation

enum Alphabet {
    case english
    case cyrillic
}

extension UnicodeScalar {
    
    var alphabet: Alphabet {
        switch self {
        case UnicodeScalar("a")...UnicodeScalar("z"), UnicodeScalar("A")...UnicodeScalar("Z"):
            return .english
        case UnicodeScalar("а")...UnicodeScalar("я"), UnicodeScalar("А")...UnicodeScalar("Я"):
            return .cyrillic
        default:
            fatalError("WRONG SYMBOL")
        }
    }

    func caesarCastValue(for alphabet: Alphabet) -> Int {
        switch alphabet {
        case .english:
            if Character(self).isUppercase {
                return Int(UnicodeScalar("A").value)
            } else {
                return Int(UnicodeScalar("a").value)
            }
        case .cyrillic:
            if Character(self).isUppercase {
                return Int(UnicodeScalar("А").value)
            } else {
                return Int(UnicodeScalar("а").value)
            }
        }
    }
    
    func power(for alphabet: Alphabet) -> Int {
        switch alphabet {
        case .english:
            return 26
        case .cyrillic:
            return 32
        }
    }
}

extension String {
    init?(_ value: UnicodeScalar?) {
        guard let unwrappedValue = value else { return nil }
        self = String(unwrappedValue)
    }
}

func encode(this string: String, with key: Int) -> String {
    let encodeShift: (UnicodeScalar) -> String? = {
        let alphabet = $0.alphabet
        let castValue = $0.caesarCastValue(for: alphabet)
        let power = $0.power(for: alphabet)
        let result = (Int($0.value) + key - castValue) % power + castValue
        return String(
            UnicodeScalar(result < 0 ? result + power : result)
        )
    }
    return string
        .filter { $0.isLetter }
        .unicodeScalars
        .compactMap(encodeShift)
        .reduce("", +)
}

func decode(this string: String, with key: Int) -> String {
    let decodeShift: (UnicodeScalar) -> String? = {
        let alphabet = $0.alphabet
        let castValue = $0.caesarCastValue(for: alphabet)
        let power = $0.power(for: alphabet)
        let result = (Int($0.value) - key + power - castValue) % power + castValue
        return String(
            UnicodeScalar(result < 0 ? result + power : result)
        )
    }
    return string
        .filter { $0.isLetter }
        .unicodeScalars
        .compactMap(decodeShift)
        .reduce("", +)
}

let string = "Hello"
let key = 10

let encodedString = encode(this: string, with: key)
print("ENCODED: ", encodedString)
print("DECODED: ", decode(this: encodedString, with: key))
