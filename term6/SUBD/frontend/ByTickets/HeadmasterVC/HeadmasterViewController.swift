//
//  HeadmasterViewController.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/11/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa

struct Transaction: Codable {
    let moneyTransfered: Int
    let card: Int
    let cardValidDate: String
    let bank: String
}

extension Transaction {
    enum CodingKeys: String, CodingKey {
        case moneyTransfered = "money_transfered"
        case card = "card_number"
        case cardValidDate = "card_valid_until_date"
        case bank = "bank_name"
    }
}

struct Sale: Codable {
    let date: Date
    let moneyTransfered: Int
    let user: String
    let card: Int
    let cardValidDate: String
    let bank: String
}

extension Sale {
    enum CodingKeys: String, CodingKey {
        case date
        case moneyTransfered = "money_transfered"
        case user = "from_user"
        case card = "card_number"
        case cardValidDate = "card_valid_until_date"
        case bank = "bank_name"
    }

}

class HeadmasterViewController: NSViewController {
    
    @IBOutlet weak var transactionsTableView: NSTableView!
    @IBOutlet weak var salesTableView: NSTableView!
    
    @IBOutlet weak var availableTicketsLabel: NSTextField!
    
    
    @IBAction func refreshPressed(_ sender: Any) {
        dataSourcesReload()
    }
    
    var user: User = .defaultUser(with: 0)
    
    private var transactions = [Transaction]() {
        didSet {
            transactionsTableView.reloadData()
        }
    }
    
    private var sales = [Sale]() {
        didSet {
            salesTableView.reloadData()
        }
    }
    
    private func dataSourcesReload() {
        Database.callProcedure("see_transactions", with: [user.username]) {
            [weak self] (res: Result<[Transaction], RequestError>) -> Void in
            guard let self = self else { return }
            switch res {
            case .success(let transactions):
                self.transactions = transactions
            case .failure(let error):
                self.showAlert(title: "TICKETS LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
            }
        }
        Database.callProcedure("see_sales", with: [user.username]) {
            [weak self] (res: Result<[Sale], RequestError>) -> Void in
            guard let self = self else { return }
            switch res {
            case .success(let sales):
                self.sales = sales
            case .failure(let error):
                self.showAlert(title: "TICKETS LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
            }
        }
        Database.callProcedure("amount_of_tickets", with: [user.username]) {
            [weak self] (res: Result<[Count], RequestError>) -> Void in
            guard let self = self else { return }
            switch res {
            case .success(let amount):
                self.availableTicketsLabel.stringValue = "\(amount.first?.amount ?? 0)"
            case .failure(let error):
                self.showAlert(title: "TICKETS LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        transactionsTableView.delegate = self
        transactionsTableView.dataSource = self
        salesTableView.delegate = self
        salesTableView.dataSource = self
        dataSourcesReload()
    }
    
}

extension HeadmasterViewController: NSTableViewDataSource {
    func numberOfRows(in tableView: NSTableView) -> Int {
        if tableView == salesTableView {
            return sales.count
        } else {
            return transactions.count
        }
    }
}

extension HeadmasterViewController: NSTableViewDelegate {
    private enum CellIdentifiers {
        static let date = "dateCell"
        static let money = "moneyCell"
        static let user = "fromUserCell"
        static let card = "cardCell"
        static let cardValid = "cardValidCell"
        static let bank = "bankCell"
    }
    
    func setupSaleCell(for tableView: NSTableView, and tableColumn: NSTableColumn?, with sale: Sale) -> NSView? {
        var text: String = ""
        var cellIdentifier: String = ""
        var cell: NSTableCellView
        
        if tableColumn == tableView.tableColumns[0] {
            text = "\(sale.date)"
            cellIdentifier = CellIdentifiers.date
        } else if tableColumn == tableView.tableColumns[1] {
            text = "\(sale.moneyTransfered)"
            cellIdentifier = CellIdentifiers.money
        } else if tableColumn == tableView.tableColumns[2] {
            text = "\(sale.user)"
            cellIdentifier = CellIdentifiers.user
        } else if tableColumn == tableView.tableColumns[3] {
            text = "\(sale.card)"
            cellIdentifier = CellIdentifiers.card
        } else if tableColumn == tableView.tableColumns[4] {
            text = "\(sale.cardValidDate)"
            cellIdentifier = CellIdentifiers.cardValid
        } else if tableColumn == tableView.tableColumns[5] {
            text = "\(sale.bank)"
            cellIdentifier = CellIdentifiers.bank
        } else {
            return nil
        }
        cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: self) as! NSTableCellView
        cell.textField?.stringValue = text
        return cell
    }
    
    func setupTransactionCell(for tableView: NSTableView, and tableColumn: NSTableColumn?, with transaction: Transaction) -> NSView? {
        var text: String = ""
        var cellIdentifier: String = ""
        var cell: NSTableCellView
        
        if tableColumn == tableView.tableColumns[0] {
            text = "\(transaction.moneyTransfered)"
            cellIdentifier = CellIdentifiers.money
        } else if tableColumn == tableView.tableColumns[1] {
            text = "\(transaction.card)"
            cellIdentifier = CellIdentifiers.card
        } else if tableColumn == tableView.tableColumns[2] {
            text = "\(transaction.cardValidDate)"
            cellIdentifier = CellIdentifiers.cardValid
        } else if tableColumn == tableView.tableColumns[3] {
            text = "\(transaction.bank)"
            cellIdentifier = CellIdentifiers.bank
        } else {
            return nil
        }
        cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: self) as! NSTableCellView
        cell.textField?.stringValue = text
        return cell
    }
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        if tableView == salesTableView {
            return setupSaleCell(for: tableView, and: tableColumn, with: sales[row])
        } else if tableView == transactionsTableView {
            return setupTransactionCell(for: tableView, and: tableColumn, with: transactions[row])
        } else {
            return nil
        }
    }
    
}

extension HeadmasterViewController: Userable {}
