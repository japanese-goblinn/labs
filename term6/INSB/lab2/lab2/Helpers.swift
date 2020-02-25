//
//  Helpers.swift
//  lab2
//
//  Created by Kirill Gorbachyonok on 2/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

extension String {
    var date: Date? {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd hh:mm:ss"
        return dateFormatter.date(from: self)
    }
}

extension Date {
    var string: String {
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "yyyy-MM-dd hh:mm:ss"
        return dateFormatter.string(from: self)
    }
}

struct AuthenticationBlock: Codable {
    let clientId: String
    let timeStamp: Date
    
    var jsonString: String {
        let json = try! JSONEncoder().encode(self)
        return String(data: json, encoding: .utf8)!
    }
}

struct Ticket: Codable {
    let clientId: String
    let resourceId: String
    let timeStamp: Date
    let validPeriod: TimeInterval
    let key: String
    
    var jsonString: String {
        let json = try! JSONEncoder().encode(self)
        return String(data: json, encoding: .utf8)!
    }
}

struct Response: Codable {
    let encodedTicket: String
    let key: String
    
    var jsonString: String {
        let json = try! JSONEncoder().encode(self)
        return String(data: json, encoding: .utf8)!
    }
}

enum KerberosError: Error {
    case resoureFailure(description: String)
    case timeExpired(description: String)
}
