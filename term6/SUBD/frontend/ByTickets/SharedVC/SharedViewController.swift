//
//  SharedViewController.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/11/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa

class SharedViewController: NSViewController {

    var user: User = .defaultUser(with: 0)
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
}

extension SharedViewController: Userable {}
