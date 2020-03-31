//
//  Controller.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/26/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa
import SwiftUI

extension View  {
    func openInNewWindow(_ title: String) {
        let window = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 1010, height: 600),
            styleMask: [.titled, .resizable],
            backing: .buffered,
            defer: false)
        window.contentView = NSHostingView(rootView: self)
        window.center()
        window.title = title
        (NSApplication.shared.delegate as! AppDelegate).window.close()
        window.makeKeyAndOrderFront(nil)
    }
}
