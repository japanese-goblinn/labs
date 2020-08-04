//
//  Date+Extension.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/28/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

extension Date {
    var string: String {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd HH:mm:ss"
        return dateFormatter.string(from: self)
    }
}
