//
//  File.swift
//  mzi
//
//  Created by Kirill Gorbachyonok on 9/13/20.
//

import Foundation

func readFromFile(named fileName: String, withExtension: String) -> [String]? {
    guard
        var filepath = FileManager.default.urls(for: .downloadsDirectory, in: .userDomainMask).first
    else {
        return nil
    }
    filepath.appendPathComponent("labs/term7/mzi/\(fileName).\(withExtension)")
    return try? String(contentsOf: filepath, encoding: .utf8).components(separatedBy: .newlines)
}
