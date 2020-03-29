//
//  String+Extenstion.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/26/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

extension String {
    var isNotEmpty: Bool { !self.isEmpty }
}

extension String {
    var date: Date? {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ssZ"
        return dateFormatter.date(from: self)
    }
}
