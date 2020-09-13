//
//  String+Extension.swift
//  mzi
//
//  Created by Kirill Gorbachyonok on 9/13/20.
//

import Foundation

extension String {
    
    var binary: Self { self.unicodeScalars.map { $0.value }.map(DES.binaryConvert).reduce("", +) }
    
    var humanReadable: Self {
        DES.separateOnBlocks(of: 8, this: self)
            .compactMap { UInt8($0, radix: 2) }
            .map { String(UnicodeScalar($0)) }
            .reduce("", +)
    }
    
    func returnBlocks(of size: Int) -> [Self] {
        return stride(from: 0, to: self.count, by: size).map { self[$0..<$0 + size] }
    }
    
    static func <<(lhs: Self, rhs: Int) -> Self {
        return lhs[rhs..<lhs.count] + lhs[0..<rhs]
    }
    
    static func ^(lhs: Self, rhs: Self) -> Self {
        return zip(lhs, rhs).map { Self((Int(Self($0))!) ^ Int(Self($1))!) }.reduce("", +)
    }
    
    subscript(i: Int) -> String {
        let index: String.Index
        if i < 0 {
            index = self.index(self.endIndex, offsetBy: i)
        } else {
            index = self.index(self.startIndex, offsetBy: i)
        }
        return String(self[index])
    }
    
    subscript(i: Range<Int>) -> String {
        let start = i.first!
        let end = i.last!
        let startIndex = self.index(self.startIndex, offsetBy: start)
        let endIndex = self.index(self.startIndex, offsetBy: end)
        return String(self[startIndex...endIndex])
    }
        
    subscript(i: ClosedRange<Int>) -> String {
        let start = i.first!
        let end = i.last!
        let startIndex = self.index(self.startIndex, offsetBy: start)
        let endIndex = self.index(self.startIndex, offsetBy: end)
        return String(self[startIndex...endIndex])
    }
}
