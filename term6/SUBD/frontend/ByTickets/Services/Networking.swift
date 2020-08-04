//
//  Networking.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation

class Networking {
    
    private static let baseURLString = "http://localhost:8000/api/"
    
    static func createRequest<T: Codable>(
        to path: String...,
        body: T,
        httpMethod: String = "POST"
    ) -> URLRequest? {
        let urlString = baseURLString + path.joined(separator: "/")
        guard let url = URL(string: urlString) else { return nil }
        var request = URLRequest(url: url)
        request.httpMethod = httpMethod
        if httpMethod == "POST" {
            request.setValue("application/json", forHTTPHeaderField: "Content-Type")
            let httpBody = try! JSONEncoder().encode(body)
            request.httpBody = httpBody
        }
        return request
    }
    
    static func request(
        _ request: URLRequest,
        handler: @escaping (Result<Data, RequestError>) -> Void
    ) {
        URLSession.shared.dataTask(with: request) { data, _, _ in
            var result: Result<Data, RequestError>
            guard let data = data else { return }
            do {
                let error = try JSONDecoder().decode(RequestError.self, from: data)
                result = .failure(error)
            } catch {
                result = .success(data)
            }
            DispatchQueue.main.async {
                handler(result)
            }
        }.resume()
    }
}
