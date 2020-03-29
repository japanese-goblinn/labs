//
//  Database.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/16/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

class Database {
    
    private static func execute(
        _ query: String,
        handler: @escaping (Result<String, RequestError>) -> Void
    ) {
        guard let request = Networking.createRequest(to: "customQuery", query: query) else {
            handler(.failure("BAD URL"))
            return
        }
        Networking.request(request) { result in
            switch result {
            case .success(let data):
                let dataString = String(decoding: data, as: UTF8.self)
                if dataString == "[]" {
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
    
    private static func executeSelect(
        _ query: String,
        handler: @escaping (Result<[User], RequestError>) -> Void
    ) {
        guard let request = Networking.createRequest(to: "usersQuery", query: query) else {
            handler(.failure("BAD URL"))
            return
        }
        Networking.request(request) { result in
            switch result {
            case .success(let data):
                guard let users = try? JSONDecoder.timestampValidDecoder.decode([User].self, from: data) else {
                    handler(.failure("BAD JSON"))
                    return
                }
                handler(.success(users))
            case .failure(let error):
                handler(.failure(error))
            }
        }
    }
    
    static func fetchUsers(handler: @escaping (Result<[User], RequestError>) -> Void) {
        executeSelect(
            "SELECT u.*, l.lastname, f.firstname FROM user AS u JOIN lastname as l ON u.lastname_id = l.id JOIN firstname as f ON u.firstname_id = f.id"
        ) {
            handler($0)
        }
    }
    
    static func insert(_ user: User, handler: @escaping (Result<String, RequestError>) -> Void) {
        execute(
            "INSERT INTO user (username, email, last_login, password_hash, role, firstname_id, lastname_id) VALUES ('\(user.username)', '\(user.email)', '\(user.lastLogin.string)', '\(user.passwordHashValue)', '\(user.role.rawValue)', '1', '1')"
        ) {
            handler($0)
        }
    }
    
    static func update(_ user: User, handler: @escaping (Result<String, RequestError>) -> Void) {
        execute(
            "UPDATE user SET username='', email='', last_login='', password_hash='', role='', firstname_id, lastname_id) VALUES ('\(user.username)', '\(user.email)', '\(user.lastLogin.string)', '\(user.passwordHashValue)', '\(user.role.rawValue)', '1', '1')"
        ) {
            handler($0)
        }
    }
    
    static func delete(_ user: User) { }
    
    static func search(_ searchParams: String, handler: @escaping (Result<[User], RequestError>) -> Void) {
        executeSelect(
            "SELECT u.*, l.lastname, f.firstname FROM user AS u JOIN lastname as l ON u.lastname_id = l.id JOIN firstname as f ON u.firstname_id = f.id WHERE " + searchParams
        ) {
            handler($0)
        }
    }
}
