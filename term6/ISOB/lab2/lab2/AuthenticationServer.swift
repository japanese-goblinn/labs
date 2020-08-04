//
//  AuthenticationServer.swift
//  lab2
//
//  Created by Kirill Gorbachyonok on 2/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

struct AuthenticationServer {
    
    private var keys = [SecretKeyType: String]()

    mutating func add(this key: String, type: SecretKeyType) { keys[type] = key }
    
    func requestTicket(for client: Client, resourceID: UUID) -> String {
        let ticket = Ticket(
            clientId: client.id.uuidString,
            resourceId: resourceID.uuidString,
            timeStamp: Date(),
            validPeriod: 5,
            key: "sessions-tgs-secret-key"
        )
        let encodedTicket = DES.encode(this: ticket.jsonString, using: keys[.tgs]!)
        let response = Response(encodedTicket: encodedTicket, key: "sessions-tgs-secret-key")
        return DES.encode(this: response.jsonString, using: client.key)
    }
}

extension AuthenticationServer {
    enum SecretKeyType {
        case client
        case server
        case tgs
    }
}
