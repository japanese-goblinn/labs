//
//  SharedViewController.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/11/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa

fileprivate enum Filter: String, CaseIterable {
    case nothing
    case movie
    case cinema
    case date
}

class SharedViewController: NSViewController {

    @IBOutlet private weak var availableTicketsTableView: NSTableView!
    @IBOutlet private weak var myTicketsTableView: NSTableView!
    
    @IBOutlet private weak var searchField: NSSearchField!
    
    @IBOutlet weak var filterByPopup: NSPopUpButton!
    
    @IBOutlet weak var filterValueTextFiled: NSTextField!
    
    @IBAction func refreshAllButtonPressed(_ sender: Any) {
        dataSourcesReload()
    }
    
    var user: User = .defaultUser(with: 0)
    
    private var availableTickets = [Ticket]() {
        didSet {
            availableTicketsTableView.reloadData()
        }
    }
    
    private var myTickets = [Ticket]() {
        didSet {
            myTicketsTableView.reloadData()
        }
    }
    
    @IBAction func byTicket(_ sender: Any) {
        guard
            availableTickets.indices.contains(availableTicketsTableView.selectedRow)
        else { return }
        let ticket = availableTickets[availableTicketsTableView.selectedRow]
        Database.callVoidProcedure("buy_ticket", with: [user.username, "\(ticket.id)"]) { [weak self] error in
            guard let self = self else { return }
            guard let error = error else {
                self.dataSourcesReload()
                return
            }
            self.showAlert(title: error.error, content: error.error, buttonText: "OK", style: .critical)
        }
    }
    
    private func dataSourcesReload() {
        Database.callProcedure("see_available_tickets", with: []) {
            [weak self] (res: Result<[Ticket], RequestError>) -> Void in
            guard let self = self else { return }
            switch res {
            case .success(let tickets):
                self.availableTickets = tickets
            case .failure(let error):
                self.showAlert(title: "TICKETS LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
            }
        }
        Database.callProcedure("see_my_tickets", with: [user.username]) {
            [weak self] (res: Result<[Ticket], RequestError>) -> Void in
            guard let self = self else { return }
            switch res {
            case .success(let tickets):
                self.myTickets = tickets
            case .failure(let error):
                self.showAlert(title: "TICKETS LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        availableTicketsTableView.delegate = self
        availableTicketsTableView.dataSource = self
        myTicketsTableView.delegate = self
        myTicketsTableView.dataSource = self
        searchField.delegate = self
        filterValueTextFiled.delegate = self
        
        filterByPopup.removeAllItems()
        filterByPopup.addItems(withTitles: Filter.allCases.map(\.rawValue))
        filterByPopup.selectItem(at: 0)
        
        dataSourcesReload()
    }
    
}

extension SharedViewController: NSTableViewDataSource {
    func numberOfRows(in tableView: NSTableView) -> Int {
        if tableView == availableTicketsTableView {
            return availableTickets.count
        } else {
            return myTickets.count
        }
    }
}

extension SharedViewController: NSTableViewDelegate {
    
    private enum CellIdentifiers {
        static let id = "idCell"
        static let price = "priceCell"
        static let starts = "startsCell"
        static let cinema = "cinemaCell"
        static let movie = "movieCell"
        static let hall = "hallCell"
        static let row = "rowCell"
        static let seat = "seatCell"
    }
    
    func setupCell(for tableView: NSTableView, and tableColumn: NSTableColumn?, with ticket: Ticket) -> NSView? {
        var text: String = ""
        var cellIdentifier: String = ""
        var cell: NSTableCellView
        
        if tableColumn == tableView.tableColumns[0] {
            text = "\(ticket.id)"
            cellIdentifier = CellIdentifiers.id
        } else if tableColumn == tableView.tableColumns[1] {
            text = "\(ticket.price)"
            cellIdentifier = CellIdentifiers.price
        } else if tableColumn == tableView.tableColumns[2] {
            text = "\(ticket.starts)"
            cellIdentifier = CellIdentifiers.starts
        } else if tableColumn == tableView.tableColumns[3] {
            text = "\(ticket.cinema)"
            cellIdentifier = CellIdentifiers.cinema
        } else if tableColumn == tableView.tableColumns[4] {
            text = "\(ticket.movie)"
            cellIdentifier = CellIdentifiers.movie
        } else if tableColumn == tableView.tableColumns[5] {
            text = "\(ticket.hall)"
            cellIdentifier = CellIdentifiers.hall
        } else if tableColumn == tableView.tableColumns[6] {
            text = "\(ticket.row)"
            cellIdentifier = CellIdentifiers.row
        } else if tableColumn == tableView.tableColumns[7] {
            text = "\(ticket.seat)"
            cellIdentifier = CellIdentifiers.seat
        } else {
            return nil
        }
        cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: self) as! NSTableCellView
        cell.textField?.stringValue = text
        return cell
    }
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        if tableView == availableTicketsTableView {
            return setupCell(for: tableView, and: tableColumn, with: availableTickets[row])
        } else if tableView == myTicketsTableView {
            return setupCell(for: tableView, and: tableColumn, with: myTickets[row])
        } else {
            return nil
        }
    }
}

extension SharedViewController: NSSearchFieldDelegate {
       
    func control(_ control: NSControl, textView: NSTextView, doCommandBy commandSelector: Selector) -> Bool {
        guard commandSelector == #selector(insertNewline) else { return false }
        guard textView.string.isNotEmpty else {
            dataSourcesReload()
            return false
        }
        if control as? NSSearchField != nil {
            let query = textView.string.components(separatedBy: .whitespaces)
            let procName = "find_tickets_by_" + query[0]
            Database.callProcedure(procName, with: [query.dropFirst().joined(separator: " ")]) {
                [weak self] (res: Result<[Ticket], RequestError>) -> Void in
                guard let self = self else { return }
                switch res {
                case .success(let tickets):
                    self.availableTickets = tickets
                case .failure(let error):
                    self.showAlert(title: "TICKETS LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
                }
            }
        } else {
            let query = textView.string
                .components(separatedBy: .whitespaces)
                .filter(\.isNotEmpty)
                .joined(separator: " ")
            guard let name = Filter.init(rawValue: filterByPopup.selectedItem!.title) else { return false }
            var procName = "filter_tickets_by_" + name.rawValue
            var args = [query]
            if name == .nothing {
                procName = "see_available_tickets"
                args.removeAll()
            }
            Database.callProcedure(procName, with: args) {
                [weak self] (res: Result<[Ticket], RequestError>) -> Void in
                guard let self = self else { return }
                switch res {
                case .success(let tickets):
                    self.availableTickets = tickets
                case .failure(let error):
                    self.showAlert(title: "TICKETS LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
                }
            }
        }
        return true
    }
}

extension SharedViewController: Userable {}
