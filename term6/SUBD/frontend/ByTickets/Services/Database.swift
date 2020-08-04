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
        handler: @escaping (Result<String, RequestError>) -> Void = { print("\(#function): \($0)") }
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
        handler: @escaping (Result<User, RequestError>) -> Void
    ) {
        fetchUsers(
            "WHERE username='\(username)' AND password_hash='\(password.hashValue)'"
        ) { (res: Result<[User], RequestError>) -> Void in
            switch res {
            case .failure(let error):
                handler(.failure(error))
                logAuthentication(username: username, password: password.hashValue, status: .error)
            case .success(let users):
                guard let user = users.first else {
                    handler(.failure("WRONG USERNAME OR PASSWORD"))
                    logAuthentication(username: username, password: password.hashValue, status: .error)
                    return
                }
                executeSelect(
                    "SELECT * FROM ban WHERE blocked_username='\(user.username)'"
                ) { (res: Result<[Ban], RequestError>) -> Void in
                    switch res {
                    case .success(let bans):
                        if bans.isEmpty {
                            handler(.success(user))
                            logAuthentication(username: username, password: password.hashValue, status: .ok)
                        } else {
                            let ban = bans.first!
                            if ban.isBlocked.contains("yes") {
                                handler(.failure("YOU ARE BANNED FOR \(bans.first!.duration)"))
                                logAuthentication(username: username, password: password.hashValue, status: .error)
                            } else {
                                handler(.success(user))
                                logAuthentication(username: username, password: password.hashValue, status: .ok)
                            }
                        }
                    case .failure(let err):
                        handler(.failure(err))
                        logAuthentication(username: username, password: password.hashValue, status: .error)
                    }
                }
            }
        }
    }
    
    static func callProcedure<T: Codable>(
        _ procName: String,
        with params: [String],
        handler: @escaping (Result<[T], RequestError>) -> Void
    ) {
        let r = ProcedureRequest(procedureName: procName, params: params)
        guard let request = Networking.createRequest(to: "procedure", body: r) else {
            handler(.failure("BAD URL"))
            return
        }
        Networking.request(request) { res in
            switch res {
            case .success(let data):
                print(String(decoding: data, as: UTF8.self))
                do {
                    let decodedData = try JSONDecoder.timestampValidDecoder.decode([T].self, from: data)
                    handler(.success(decodedData))
                } catch {
                    do {
                        let error = try JSONDecoder.timestampValidDecoder.decode([RequestError].self, from: data)
                        handler(.failure(error.first!))
                    } catch {
                        print(error.localizedDescription)
                    }
                }
            case .failure(let error):
                handler(.failure(error))
            }
        }
    }
    
    static func loadMovies(handler: @escaping (Result<[Movie], RequestError>) -> Void) {
        executeSelect("SELECT * FROM movie") { handler($0) }
    }
    
    static func callVoidProcedure(
        _ procName: String,
        with params: [String],
        handler: @escaping (RequestError?) -> Void
    ) {
        let r = ProcedureRequest(procedureName: procName, params: params)
        guard let request = Networking.createRequest(to: "procedure", body: r) else {
            handler("BAD URL")
            return
        }
        Networking.request(request) { res in
            switch res {
            case .success(let data):
                do {
                    let err = try JSONDecoder.timestampValidDecoder.decode(RequestError.self, from: data)
                    handler(err)
                } catch {
                    handler(nil)
                }
            case .failure(let error):
                handler(error)
            }
        }
    }
    
        
    static func fetchUsers(
        _ appending: String = "",
        handler: @escaping (Result<[User], RequestError>) -> Void
    ) {
        executeSelect(
            "SELECT u.*, l.lastname, f.firstname, b.is_blocked FROM user AS u JOIN lastname as l ON u.lastname_id = l.id JOIN firstname as f ON u.firstname_id = f.id LEFT JOIN ban as b ON u.username = b.blocked_username " + appending
        ) { handler($0) }
    }
    
    static func logAuthentication(username: String, password: Int, status: AuthenticationStatus) {
        execute(
            "INSERT INTO login (`when`, status, username, password_hash) VALUES (NOW(), '\(status)', '\(username)', '\(password)')"
        )
    }
    
    static func search(_ searchParams: String, handler: @escaping (Result<[User], RequestError>) -> Void) {
        fetchUsers("WHERE \(searchParams)") { handler($0) }
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
                    ) {
                        switch $0 {
                        case .success(_):
                            execute(
                                "INSERT INTO ban (blocked_username, is_blocked) VALUES ('\(user.username)', '\(user.banned?.rawValue ?? "no")')"
                            )
                        case .failure(_):
                            break
                        }
                        handler($0)
                    }
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
                    ) { handler($0) }
                    execute(
                        "UPDATE ban SET is_blocked='\(user.banned?.rawValue ?? "no")', duration='forever' WHERE username='\(user.username)'"
                    )
                } catch {
                    handler(.failure("\(error.localizedDescription)"))
                }
            case .failure(let error):
                handler(.failure(error))
            }
        }
    }
    
    static func delete(
        _ user: User,
        handler: @escaping (Result<String, RequestError>
    ) -> Void) {
        execute("DELETE FROM user WHERE username='\(user.username)'") { handler($0) }
    }
}

extension Database {
    enum AuthenticationStatus: String {
        case ok
        case error
    }
}
