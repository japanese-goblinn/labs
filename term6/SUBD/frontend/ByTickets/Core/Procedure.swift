//
//  Procedure.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/9/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

struct ProcedureRequest<T: Codable>: Codable {
    let procedureName: String
    let params: [T]
}

struct ProcedureResult: Codable {
    let firstnameID: Int
    let lastnameID: Int
}
