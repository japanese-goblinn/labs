//
//  ManagerViewController.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/11/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa

struct Count: Codable {
    let amount: Int
}

class ManagerViewController: NSViewController {
    
    var user: User = .defaultUser(with: 0)
    
    @IBOutlet weak var ticketsAmount: NSTextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        Database.callProcedure("amount_of_tickets", with: [user.username]) {
            [weak self] (res: Result<[Count], RequestError>) -> Void in
            guard let self = self else { return }
            switch res {
            case .success(let amount):
                self.ticketsAmount.stringValue = "\(amount.first?.amount ?? 0)"
            case .failure(let error):
                self.showAlert(title: "TICKETS LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
            }
        }
    }
}

extension ManagerViewController: Userable {}
