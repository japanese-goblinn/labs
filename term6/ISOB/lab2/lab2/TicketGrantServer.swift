//
//  TicketGrantServer.swift
//  lab2
//
//  Created by Kirill Gorbachyonok on 2/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

struct TicketGrantServer {
    let id: UUID
    let key: String
    private var serverKey: String?
    private var savedClientToTGSKey: String!
    
    init(id: UUID, key: String) {
        self.id = id
        self.key = key
    }
    
    func genarateResponse(on resourceID: UUID, for clientID: UUID) -> String? {
        guard let key = serverKey else {
            print("ERROR. No server key was granted")
            return nil
        }
        let ticket = Ticket(
            clientId: clientID.uuidString,
            resourceId: resourceID.uuidString,
            timeStamp: Date(),
            validPeriod: 3,
            key: "session-server-secret-key"
        )
        let encodedTicket = DES.encode(this: ticket.jsonString, using: key)
        let response = Response(encodedTicket: encodedTicket, key: "session-server-secret-key")
        return DES.encode(this: response.jsonString, using: savedClientToTGSKey)
    }
    
    mutating func requestTGSTicket(ticket: String, authBlock: String, resource: Server) throws {
        let decodedTicketString = DES.decode(this: ticket, using: key)
        let decodedTicket = try JSONDecoder().decode(
            Ticket.self,
            from: Data(decodedTicketString.utf8)
        )
        guard decodedTicket.resourceId == id.uuidString else {
            throw KerberosError.resoureFailure(description: "Wrong resource ID in TGT requested")
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
        savedClientToTGSKey = decodedTicket.key
        serverKey = resource.key
    }
}
