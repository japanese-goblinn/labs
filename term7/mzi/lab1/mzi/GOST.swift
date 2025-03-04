//
//  GOST.swift
//  mzi
//
//  Created by Kirill Gorbachyonok on 9/28/20.
//

import Foundation

class GOST {
    private static let s = [
        4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3,
        14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9,
        5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11,
        7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3,
        6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 3, 11, 2,
        4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14,
        13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12,
        1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12
    ]
    
    public static func encode(this string: String, using key: String) -> String {
        assert(key.count >= 256, "KEY LENGTH ERROR")
        let allKeys = keys(for: key)
        let result = separateOnBlocks(of: 64, this: string.binary).map { s -> String in
            let withPermutations = ip.map({ s[$0 - 1] }).reduce("", +)
            var rounds = [separateOnBlocks(of: 32, this: withPermutations)]
            allKeys.enumerated().forEach { i, key in
                let (prevLi, prevRi) = (rounds[i][0], rounds[i][1])
                let li = prevRi
                let ri = prevLi ^ f(prevRi, key)
                rounds.append([li, ri])
            }
            let encoded = rounds.last!.reduce("", +)
            return fp.map({ encoded[$0 - 1] }).reduce("", +)
        }
        return result.reduce("", +).humanReadable
    }
    
    public static func decode(this string: String, using key: String) -> String {
        assert(key.count >= 256, "KEY LENGTH ERROR")
        let allKeys = keys(for: key).reversed()
        let result = separateOnBlocks(of: 64, this: string.binary).map { s -> String in
            let withPermutations = ip.map({ s[$0 - 1] }).reduce("", +)
            var rounds = [separateOnBlocks(of: 32, this: withPermutations)]
            allKeys.enumerated().forEach { i, key in
                let (prevLi, prevRi) = (rounds[i][0], rounds[i][1])
                let ri = prevLi
                let li = prevRi ^ f(prevLi, key)
                rounds.append([li, ri])
            }
            let encoded = rounds.last!.reduce("", +)
            return fp.map({ encoded[$0 - 1] }).reduce("", +)
        }
        return result.reduce("", +).humanReadable.filter { $0 != "\0" }
    }
}
