//
//  Server.swift
//  lab2
//
//  Created by Kirill Gorbachyonok on 2/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

struct Server {
    let id: UUID
    let key: String
    private var savedClientToServerKey: String?
    private var clientTime: Date?
    
    init(id: UUID, key: String) {
        self.id = id
        self.key = key
    }
    
    func response() -> String? {
        guard
            let key = savedClientToServerKey,
            let time = clientTime
        else {
            print("ERROR. Ticket request failed")
            return nil
        }
        return DES.encode(this: time.addingTimeInterval(1).string, using: key)
    }
    
    mutating func requestTicket(ticket: String, authBlock: String) throws {
        let decodedTicketString = DES.decode(this: ticket, using: key)
        let decodedTicket = try JSONDecoder().decode(
            Ticket.self,
            from: Data(decodedTicketString.utf8)
        )
        guard decodedTicket.resourceId == id.uuidString else {
            throw KerberosError.resoureFailure(description: "Wrong resource ID in TGS requested")
        }
        guard decodedTicket.timeStamp.addingTimeInterval(decodedTicket.validPeriod) >= Date() else {
            throw KerberosError.timeExpired(description: "Ticket validation time expired")
        }
        let decodedAuthBlockString = DES.decode(this: authBlock, using: decodedTicket.key)
        let decodedAuthBlock = try JSONDecoder().decode(
            AuthenticationBlock.self,
            from: Data(decodedAuthBlockString.utf8)
        )
        guard decodedAuthBlock.clientId == decodedTicket.clientId else {
            throw KerberosError.resoureFailure(description: "Ticket clientId not equal to auth block clientId")
        }
        guard decodedTicket.timeStamp.addingTimeInterval(decodedTicket.validPeriod) >= decodedAuthBlock.timeStamp else {
            throw KerberosError.timeExpired(description: "Authentication time expired")
        }
        savedClientToServerKey = decodedTicket.key
        clientTime = decodedAuthBlock.timeStamp
    }

}
