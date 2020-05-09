//
//  UsersViewController.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/26/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa

class UsersViewController: NSViewController {
    
    @IBOutlet private weak var tableView: NSTableView!
    @IBOutlet private weak var saveChangesButton: NSButton!
    @IBOutlet private weak var searchField: NSSearchField!
    
    private let defaultTextColor: NSColor = .controlTextColor
    private let changedCellColor: NSColor = .systemYellow
    private let deletedCellColor: NSColor = .systemRed
    private let insertedCellColor: NSColor = .systemGreen
    
    private var isUserExist: Bool { users.indices.contains(tableView.selectedRow) }
    private var isDBUser: Bool {
        users.indices.contains(tableView.selectedRow) && !users[tableView.selectedRow].username.isEmpty
    }
    
    private var users = [User]()
    
    private var diffUsers = [Int: Operation]() {
        willSet {
            saveChangesButton.isHidden = newValue.isEmpty
        }
    }
    
    @IBAction func refresh(_ sender: Any) {
        updateDataSource()
    }
    
    @IBAction func addUser(_ sender: Any) {
        guard let lastId = users.isEmpty ? 0 : users.last?.id else { return }
        tableView.beginUpdates()
        diffUsers[tableView.numberOfRows] = .insert
        users += [.defaultUser(with: lastId + 1)]
        tableView.insertRows(at: IndexSet(integer: tableView.numberOfRows), withAnimation: .slideUp)
        tableView.endUpdates()
    }
    
    @IBAction func banFor30SecondsClicked(_ sender: Any) {
        guard isDBUser else { return }
        let username = users[tableView.selectedRow].username
        Database.execute(
            "UPDATE ban SET is_blocked='yes', duration='30 sec' WHERE blocked_username='\(username)'"
        ) { [weak self] res in
            switch res {
            case .success(_):
                Database.execute(
                    "CREATE EVENT IF NOT EXISTS block_event_\(username) ON SCHEDULE AT CURRENT_TIMESTAMP + INTERVAL 30 SECOND DO UPDATE ban SET is_blocked='no' WHERE blocked_username='\(username)';"
                )
            case .failure(let error):
                self?.showAlert(title: "BAN ERROR", content: error.reason, buttonText: "OK", style: .critical)
            }
        }
    }
    
    @IBAction func banFor60SecondsClicked(_ sender: Any) {
        guard isDBUser else { return }
        let username = users[tableView.selectedRow].username
        Database.execute(
            "UPDATE ban SET is_blocked='yes', duration='60 sec' WHERE blocked_username='\(username)'"
        ) { [weak self] res in
            switch res {
            case .success(_):
                Database.execute(
                    "CREATE EVENT IF NOT EXISTS block_event_\(username) ON SCHEDULE AT CURRENT_TIMESTAMP + INTERVAL 60 SECOND DO UPDATE ban SET is_blocked='no' WHERE blocked_username='\(username)';"
                )
            case .failure(let error):
                self?.showAlert(title: "BAN ERROR", content: error.reason, buttonText: "OK", style: .critical)
            }
        }
    }
    
    @IBAction func banFor5MinutesClicked(_ sender: Any) {
        guard isDBUser else { return }
        let username = users[tableView.selectedRow].username
        Database.execute(
            "UPDATE ban SET is_blocked='yes', duration='5 minutes' WHERE blocked_username='\(username)'"
        ) { [weak self] res in
            switch res {
            case .success(_):
                Database.execute(
                    "CREATE EVENT IF NOT EXISTS block_event_\(username) ON SCHEDULE AT CURRENT_TIMESTAMP + INTERVAL 5 MINUTE DO UPDATE ban SET is_blocked='no' WHERE blocked_username='\(username)';"
                )
            case .failure(let error):
                self?.showAlert(title: "BAN ERROR", content: error.reason, buttonText: "OK", style: .critical)
            }
        }
    }
    
    @IBAction func banForeverClicked(_ sender: Any) {
        guard isDBUser else { return }
        let username = users[tableView.selectedRow].username
        Database.execute(
            "UPDATE ban SET is_blocked='yes', duration='forever' WHERE blocked_username='\(username)'"
        )
    }
    
    @IBAction func editItemClicked(_ sender: Any) {
        guard isUserExist else { return }
        diffUsers[tableView.selectedRow] = .update
        updateRow(at: tableView.selectedRow, with: .update, editable: true)
    }
    
    @IBAction func deleteItemClicked(_ sender: Any) {
        guard isUserExist else { return }
        diffUsers[tableView.selectedRow] = .delete
        updateRow(at: tableView.selectedRow, with: .delete, editable: false)
    }
    
    @IBAction func saveChanges(_ sender: Any) {
        var errors = [String]()
        let indexes = diffUsers.keys
        for i in indexes {
            switch diffUsers[i]! {
            case .insert:
                if users[i].username.isEmpty {
                    guard userForRow(at: i) != nil else {
                        errors += ["ROW \(i + 1): NOT VALID DATA"]
                        continue
                    }
                }
            case .update:
                if users[i].username.isEmpty {
                    guard let user = userForRow(at: i) else {
                        errors += ["ROW \(i + 1): NOT VALID DATA"]
                        continue
                    }
                    Database.insert(user) { [weak self] result in
                        switch result {
                        case .failure(let error):
                            self?.showAlert(title: "DB ERROR", content: error.reason, buttonText: "OK", style: .critical)
                        case .success(let message):
                            print(#function, message)
                        }
                    }
                } else {
                    guard let user = userForRow(at: i) else {
                        errors += ["ROW \(i + 1): NOT VALID DATA"]
                        continue
                    }
                    Database.update(user) { [weak self] result in
                        switch result {
                        case .failure(let error):
                            self?.showAlert(title: "DB UPDATE ERROR", content: error.reason, buttonText: "OK", style: .critical)
                        case .success(let message):
                            print(#function, message)
                        }
                    }
                }
            case .delete:
                break
//                if users[i].username != "" {
//                    Database.delete(users[i]) { [weak self] result in
//                        switch result {
//                        case .failure(let error):
//                            self?.showAlert(title: "DB DELETE ERROR", content: error.reason, buttonText: "OK", style: .critical)
//                        case .success(let message):
//                            print(#function, message)
//                        }
//                    }
//                }
            }
        }
        if !errors.isEmpty {
            showAlert(title: "VALIDATION ERROR", content: errors.sorted().joined(separator: "\n"), buttonText: "OK", style: .critical)
        }
    }
    
    private func showAlert(
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
    
    private func userForRow(at index: Int) -> User? {
        var id = 0
        var firstname = ""
        var lastname = ""
        var username = ""
        var email = ""
        var role: User.Role = .user
        var status: User.Banned = .no
        var lastLogin = Date()
        var registration = Date()
        var passwordHashValue = 0
        for col in 0 ..< tableView.numberOfColumns {
            guard let view = tableView.view(atColumn: col, row: index, makeIfNecessary: true) else { continue }
            if col != 6, col != 7 {
                let field = view.viewWithTag(0) as! NSTextField
                if col == 0 {
                    guard let localId = Int(field.stringValue) else { return nil }
                    id = localId
                } else if col == 1 {
                    guard let date = field.stringValue.date else { return nil }
                    lastLogin = date
                } else if col == 2 {
                    guard field.stringValue.isNotEmpty else { return nil }
                    firstname = field.stringValue
                } else if col == 3 {
                    guard field.stringValue.isNotEmpty else { return nil }
                    lastname = field.stringValue
                } else if col == 4 {
                    guard field.stringValue.isNotEmpty else { return nil }
                    username = field.stringValue
                } else if col == 5 {
                    guard field.stringValue.isNotEmpty else { return nil }
                    email = field.stringValue
                }  else if col == 8 {
                    guard let hash = Int(field.stringValue) else { return nil }
                    passwordHashValue = hash
                } else if col == 9 {
                    guard let date = field.stringValue.date else { return nil }
                    registration = date
                }
            } else {
                let button = view.viewWithTag(0) as! NSPopUpButton
                if col == 6 {
                    guard let localRole = User.Role(rawValue: button.selectedItem?.title ?? "") else { return nil}
                    role = localRole
                } else if col == 7 {
                    guard let localStatus = User.Banned(rawValue: button.selectedItem?.title ?? "") else { return nil}
                    status = localStatus
                }
            }
        }
        return User(
            id: id,
            firstname: firstname,
            lastname: lastname,
            username: username,
            email: email,
            role: role,
            banned: status,
            phone: nil,
            contryCode: nil,
            lastLogin: lastLogin,
            registration: registration,
            cardID: nil,
            passwordHashValue: passwordHashValue
        )
    }
    
    private func updateRow(at index: Int, with operation: Operation, editable state: Bool) {
        for col in 0 ..< tableView.numberOfColumns {
            guard let view = tableView.view(atColumn: col, row: index, makeIfNecessary: true) else { continue }
            if col != 6, col != 7 {
                let field = view.viewWithTag(0) as! NSTextField
                field.isEditable = state
                if col == 0 {
                    field.isEditable = false
                    update(cellView: view, with: operation)
                }
            } else {
                let button = view.viewWithTag(0) as! NSPopUpButton
                button.isEnabled = state
            }
        }
    }
        
    private func update(cellView: NSView, with operation: Operation) {
        let field = cellView.viewWithTag(0) as! NSTextField
        switch operation {
        case .delete:
            cellView.layer?.backgroundColor = deletedCellColor.cgColor
            field.textColor = .white
        case .update:
            cellView.layer?.backgroundColor = changedCellColor.cgColor
            field.textColor = .black
        case .insert:
            break
        }
    }
        
    private func updateDataSource() {
        Database.fetchUsers { [weak self] result in
            guard let self = self else { return }
            switch result {
            case .success(let users):
                self.users = users
                self.diffUsers.removeAll()
                self.tableView.reloadData()
            case .failure(let error):
                self.showAlert(title: "DB ERORR", content: error.reason, buttonText: "OK", style: .critical)
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.dataSource = self
        tableView.delegate = self
        searchField.delegate = self
        updateDataSource()
    }
}

extension UsersViewController: NSSearchFieldDelegate {
    
    private func validateSearch(_ string: String) {
        guard string.isNotEmpty else {
            updateDataSource()
            return
        }
        let query = string
            .components(separatedBy: .whitespaces)
            .filter { $0.isNotEmpty }
            .map { $0.contains("id") ? "u." + $0 : $0 }
            .joined(separator: " AND ")
        Database.search(query) { [weak self] res in
            guard let self = self else { return }
            switch res {
            case .success(let users):
                self.users = users
                self.diffUsers.removeAll()
                self.tableView.reloadData()
            case .failure(let error):
                self.showAlert(title: "DB ERROR", content: error.reason, buttonText: "OK", style: .critical)
            }
        }
    }
    
    func control(_ control: NSControl, textView: NSTextView, doCommandBy commandSelector: Selector) -> Bool {
        if commandSelector == #selector(insertNewline) {
            validateSearch(textView.string)
            return true
        }
        return false
    }
    
}

extension UsersViewController {
    private enum Operation {
        case insert
        case update
        case delete
    }
}

extension UsersViewController: NSTableViewDataSource {
    func numberOfRows(in tableView: NSTableView) -> Int {
        return users.count
    }
}

extension UsersViewController: NSTableViewDelegate {
        
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        
        var text: String = ""
        var cellIdentifier: String = ""
        var cell: NSTableCellView
        
        let user = users[row]
        if tableColumn == tableView.tableColumns[0] {
            text = "\(user.id)"
            cellIdentifier = CellIdentifiers.id
        } else if tableColumn == tableView.tableColumns[1] {
            text = "\(user.lastLogin)"
            cellIdentifier = CellIdentifiers.lastLogin
        } else if tableColumn == tableView.tableColumns[2] {
            text = "\(user.firstname)"
            cellIdentifier = CellIdentifiers.firstname
        } else if tableColumn == tableView.tableColumns[3] {
            text = "\(user.lastname)"
            cellIdentifier = CellIdentifiers.lastname
        } else if tableColumn == tableView.tableColumns[4] {
            text = "\(user.username)"
            cellIdentifier = CellIdentifiers.username
        } else if tableColumn == tableView.tableColumns[5] {
            text = "\(user.email)"
            cellIdentifier = CellIdentifiers.email
        } else if tableColumn == tableView.tableColumns[6] {
            let result = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: CellIdentifiers.role), owner: self) as! NSPopUpButton
            result.removeAllItems()
            result.addItems(withTitles: User.Role.allCases.map { $0.rawValue } )
            result.selectItem(withTitle: user.role.rawValue)
            result.isEnabled = false
            return result
        } else if tableColumn == tableView.tableColumns[7] {
            let result = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: CellIdentifiers.status), owner: self) as! NSPopUpButton
            result.removeAllItems()
            result.addItems(withTitles: User.Banned.allCases.map { $0.rawValue } )
            result.selectItem(withTitle: user.banned?.rawValue ?? "no")
            result.isEnabled = false
            return result
        } else if tableColumn == tableView.tableColumns[8] {
            text = "\(user.passwordHashValue)"
            cellIdentifier = CellIdentifiers.passwordHash
        } else if tableColumn == tableView.tableColumns[9] {
            text = "\(user.registration)"
            cellIdentifier = CellIdentifiers.registration
        } else {
            return nil
        }
        cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: self) as! NSTableCellView
        cell.layer?.backgroundColor = .clear
        cell.textField?.textColor = defaultTextColor
        cell.textField?.stringValue = text
        cell.textField?.isEditable = false
        return cell
    }
}

extension UsersViewController {
    private enum CellIdentifiers {
        static let id = "idCellId"
        static let firstname = "firstnameCellId"
        static let lastname = "lastnameCellId"
        static let username = "usernameCellId"
        static let email = "emailCellId"
        static let role = "roleCellId"
        static let status = "statusCellId"
        static let lastLogin = "lastLoginCellId"
        static let registration = "registrationCellId"
        static let passwordHash = "passwordHashCellId"
    }
}
