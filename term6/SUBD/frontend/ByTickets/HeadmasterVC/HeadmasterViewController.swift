//
//  HeadmasterViewController.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/11/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa

class HeadmasterViewController: NSViewController {

    var user: User = .defaultUser(with: 0)
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
}

extension HeadmasterViewController: Userable {}
