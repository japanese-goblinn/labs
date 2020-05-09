//
//  Ban.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/9/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

struct Ban {
    let id: Int
    let userID: Int
    let isBlocked: String
    let duration: String
}

extension Ban: Codable {
    enum CodingKeys: String, CodingKey {
        case id
        case userID = "user_id"
        case isBlocked = "is_blocked"
        case duration
    }
}
