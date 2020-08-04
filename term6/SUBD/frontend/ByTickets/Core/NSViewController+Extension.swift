//
//  NSViewController+Extension.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/12/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa

extension NSViewController {
    
    func showAlert(
        title: String,
        content: String,
        buttonText: String,
        style: NSAlert.Style
    ) {
        let alert = NSAlert()
        alert.messageText = title
        alert.informativeText = content
        alert.alertStyle = style
        alert.addButton(withTitle: buttonText)
        alert.runModal()
    }
    
}
