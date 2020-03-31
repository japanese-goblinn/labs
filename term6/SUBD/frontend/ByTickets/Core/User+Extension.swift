//
//  User+CodingKeys.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/26/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

struct User {
    let id: Int
    let firstname: String
    let lastname: String
    let username: String
    let email: String
    let role: Role
    let banned: Banned?
    let phone: Int?
    let contryCode: Int?
    let lastLogin: Date
    let cardID: Int?
    let passwordHashValue: Int
}

extension User: Codable {
    enum CodingKeys: String, CodingKey {
        case id
        case role
        case firstname
        case lastname
        case username
        case email
        case phone
        case banned = "is_blocked"
        case contryCode = "contry_code"
        case lastLogin = "last_login"
        case cardID = "default_card_id"
        case passwordHashValue = "password_hash"
    }
}

extension User {
    enum Role: String, Codable, CaseIterable {
        case admin
        case headmaster
        case manager
        case user
    }
    
    enum Banned: String, Codable, CaseIterable {
        case yes
        case no
    }
}

extension User {
    static func defaultUser(with id: Int) -> Self {
        .init(id: id, firstname: "", lastname: "", username: "", email: "", role: .user, banned:  .no, phone: nil, contryCode: nil, lastLogin: Date(), cardID: nil, passwordHashValue: 0)
    }
}
