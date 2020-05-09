//
//  Database.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/16/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

class Database {
    
    static func execute(
        _ query: String,
        handler: @escaping (Result<String, RequestError>) -> Void
    ) {
        guard let request = Networking.createRequest(to: "query", body: query) else {
            handler(.failure("BAD URL"))
            return
        }
        Networking.request(request) { result in
            switch result {
            case .success(let data):
                let dataString = String(decoding: data, as: UTF8.self)
                if dataString.contains("OK") {
                    handler(.success("QUERY OK"))
                } else {
                    guard let error = try? JSONDecoder().decode(RequestError.self, from: data) else {
                        handler(.failure("BAD JSON"))
                        return
                    }
                    handler(.failure(error))
                }
            case .failure(let error):
                handler(.failure(error))
            }
        }
    }
    
    private static func executeSelect<T: Codable>(
        _ query: String,
        handler: @escaping (Result<[T], RequestError>) -> Void
    ) {
        guard let request = Networking.createRequest(to: "query", body: query) else {
            handler(.failure("BAD URL"))
            return
        }
        Networking.request(request) { result in
            switch result {
            case .success(let data):
                print(String(decoding: data, as: UTF8.self))
                do {
                     let users = try JSONDecoder.timestampValidDecoder.decode([T].self, from: data)
                     handler(.success(users))
                } catch {
                    print(error.localizedDescription)
                }
            case .failure(let error):
                handler(.failure(error))
            }
        }
    }
    
    static func authenticate(
        username: String,
        password: String,
        handler: @escaping (Result<Bool, RequestError>) -> Void
    ) {
        executeSelect("SELECT u.*, l.lastname, f.firstname, b.is_blocked FROM user AS u JOIN lastname as l ON u.lastname_id = l.id JOIN firstname as f ON u.firstname_id = f.id LEFT JOIN ban as b ON u.id = b.user_id WHERE username='\(username)' AND password_hash='\(password.hashValue)'"
        ) { (res: Result<[User], RequestError>) -> Void in
            switch res {
            case .failure(let error):
                handler(.failure(error))
                execute("INSERT INTO login (`when`, status, username) VALUES (NOW(), 'error', '\(username)')") { _ in }
            case .success(let users):
                guard let user = users.first else {
                    handler(.failure("WRONG USERNAME OR PASSWORD"))
                    execute("INSERT INTO login (`when`, status, username) VALUES (NOW(), 'error', '\(username)')") { _ in }
                    return
                }
                executeSelect(
                    "SELECT * FROM ban WHERE user_id=\(user.id)"
                ) { (res: Result<[Ban], RequestError>) -> Void in
                    switch res {
                    case .success(let bans):
                        if bans.isEmpty {
                            handler(.success(true))
                             execute("INSERT INTO login (`when`, status, username) VALUES (NOW(), 'ok', '\(username)')") { _ in }
                        } else {
                            let ban = bans.first!
                            if ban.isBlocked.contains("yes") {
                                handler(.failure("YOU ARE BANNED FOR \(bans.first!.duration)"))
                                execute("INSERT INTO login (`when`, status, username) VALUES (NOW(), 'error', '\(username)')") { _ in }
                            } else {
                                handler(.success(true))
                                execute("INSERT INTO login (`when`, status, username) VALUES (NOW(), 'ok', '\(username)')") { _ in }
                            }
                        }
                    case .failure(let err):
                        handler(.failure(err))
                        execute("INSERT INTO login (`when`, status, username) VALUES (NOW(), 'error', '\(username)')") { _ in }
                    }
                }
            }
        }
    }
    
    static func fetchUsers(handler: @escaping (Result<[User], RequestError>) -> Void) {
        executeSelect(
            "SELECT u.*, l.lastname, f.firstname, b.is_blocked FROM user AS u JOIN lastname as l ON u.lastname_id = l.id JOIN firstname as f ON u.firstname_id = f.id LEFT JOIN ban as b ON u.id = b.user_id"
        ) { handler($0) }
    }
    
    struct ProcedureRequest<T: Codable>: Codable {
        let procedureName: String
        let params: [T]
    }
    
    struct ProcedureResult: Codable {
        let firstnameID: Int
        let lastnameID: Int
    }
    
    static func insert(_ user: User, handler: @escaping (Result<String, RequestError>) -> Void) {
        let r = ProcedureRequest(
            procedureName: "firstnameAndLastnameID", params: [user.firstname, user.lastname]
        )
        guard let request = Networking.createRequest(to: "procedure", body: r) else {
            handler(.failure("BAD URL"))
            return
        }
        Networking.request(request) { res in
            switch res {
            case .success(let data):
                do {
                    let procedureResult = try JSONDecoder().decode([ProcedureResult].self, from: data)
                    execute(
                        "INSERT INTO user (username, email, password_hash, firstname_id, lastname_id, role) VALUES ('\(user.username)', '\(user.email)', '\(user.passwordHashValue)', '\(procedureResult.first!.firstnameID)', '\(procedureResult.first!.lastnameID)', '\(user.role.rawValue)')"
                    ) { handler($0) }
                } catch {
                    handler(.failure("\(error.localizedDescription)"))
                }
            case .failure(let error):
                handler(.failure(error))
            }
        }
    }
    
    static func update(_ user: User, handler: @escaping (Result<String, RequestError>) -> Void) {
        let r = ProcedureRequest(
            procedureName: "firstnameAndLastnameID", params: [user.firstname, user.lastname]
        )
        guard let request = Networking.createRequest(to: "procedure", body: r) else {
            handler(.failure("BAD URL"))
            return
        }
        Networking.request(request) { res in
            switch res {
            case .success(let data):
                do {
                    let procedureResult = try JSONDecoder().decode([ProcedureResult].self, from: data)
                    execute(
                        "UPDATE user SET username='\(user.username)', email='\(user.email)', last_login='\(user.lastLogin.string)', password_hash='\(user.passwordHashValue)', role='\(user.role.rawValue)', firstname_id='\(procedureResult.first!.firstnameID)', lastname_id='\(procedureResult.first!.lastnameID)' WHERE id=\(user.id)"
                    ) {
                        handler($0)
                    }
                    execute("UPDATE ban SET is_blocked='no'WHERE user_id=\(user.id)") { _ in }
                } catch {
                    handler(.failure("\(error.localizedDescription)"))
                }
            case .failure(let error):
                handler(.failure(error))
            }
        }
    }
    
//    static func delete(_ user: User, handler: @escaping (Result<String, RequestError>) -> Void) {}
    
    static func search(_ searchParams: String, handler: @escaping (Result<[User], RequestError>) -> Void) {
        executeSelect(
            "SELECT u.*, l.lastname, f.firstname, b.is_blocked FROM user AS u JOIN lastname as l ON u.lastname_id = l.id JOIN firstname as f ON u.firstname_id = f.id LEFT JOIN ban as b ON u.id = b.user_id WHERE " + searchParams
        ) { handler($0) }
    }
}
