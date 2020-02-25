//
//  main.swift
//  lab2
//
//  Created by Kirill Gorbachyonok on 2/12/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

// 1
let client = Client(id: UUID(), key: "client_secret-key")
var server = Server(id: UUID(), key: "server-secret-key")
var ticketServer = TicketGrantServer(id: UUID(), key: "ticket-granting-server-secret-key")

var asServer = AuthenticationServer()
asServer.add(this: client.key, type: .client)
asServer.add(this: ticketServer.key, type: .tgs)
asServer.add(this: server.key, type: .server)

// 2
let asResponseString = asServer.requestTicket(for: client, resourceID: ticketServer.id)
let asResponse = try JSONDecoder().decode(
    Response.self,
    from: Data(DES.decode(this: asResponseString, using: client.key).utf8)
)
print("Authentication server response: ")
dump(asResponse)

// 3
let tgsAuthBlock = AuthenticationBlock(clientId: client.id.uuidString, timeStamp: Date())
let encodedTGSAuthBlock = DES.encode(this: tgsAuthBlock.jsonString, using: asResponse.key)

do {
    try ticketServer.requestTGSTicket(
        ticket: asResponse.encodedTicket,
        authBlock: encodedTGSAuthBlock,
        resource: server
    )
} catch let error as KerberosError {
    switch error {
    case let .resoureFailure(description):
        print(description)
    case let .timeExpired(description):
        print(description)
    }
}

// 4
guard let tgsResponseString = ticketServer.genarateResponse(
    on: server.id,
    for: client.id
) else {
    fatalError("Response error")
}

let tgsResponse = try JSONDecoder().decode(
    Response.self,
    from: Data(DES.decode(this: tgsResponseString, using: asResponse.key).utf8)
)
print("\nTGS response:")
dump(tgsResponse)

// 5
let requestDate = Date()
let serverAuthBlock = AuthenticationBlock(clientId: client.id.uuidString, timeStamp: requestDate)
let encodedServerAuthBlock = DES.encode(this: serverAuthBlock.jsonString, using: tgsResponse.key)

do {
    try server.requestTicket(
        ticket: tgsResponse.encodedTicket,
        authBlock: encodedServerAuthBlock
    )
} catch let error as KerberosError {
    switch error {
    case let .resoureFailure(description):
        print(description)
    case let .timeExpired(description):
        print(description)
    }
}

// 6
guard let finalResponse = server.response() else {
    fatalError("Authentication failed")
}
let decodedServerResponse = DES.decode(this: finalResponse, using: tgsResponse.key)

guard let date = decodedServerResponse.date, date <= requestDate.addingTimeInterval(1) else {
    fatalError("Authentication time failed")
}
print("\nSUCCESS AUTHENTICATION. KERBEROS OK")
