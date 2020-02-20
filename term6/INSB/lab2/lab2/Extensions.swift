//
//  Extensions.swift
//  lab2
//
//  Created by Kirill Gorbachyonok on 2/13/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

extension String {
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

