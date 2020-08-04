//
//  Ticket.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/12/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

struct Ticket {
    let id: Int
    let price: Int
    let starts: Date
    let cinema: String
    let movie: String
    let hall: Int
    let row: Int
    let seat: Int
}

extension Ticket: Codable {
    enum CodingKeys: String, CodingKey {
        case id
        case price = "ticket_price"
        case starts = "session_start_time"
        case cinema
        case movie
        case hall
        case row
        case seat
    }
}
