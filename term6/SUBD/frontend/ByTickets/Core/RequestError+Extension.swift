//
//  RequestError.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

struct RequestError: Error {
    let error: String
}

extension RequestError: ExpressibleByStringLiteral {
    init(stringLiteral value: String) {
        self.error = value
    }
}

extension RequestError: ExpressibleByStringInterpolation {}

extension RequestError: Decodable {}
