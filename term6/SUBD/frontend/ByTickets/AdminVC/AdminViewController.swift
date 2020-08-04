//
//  AdminViewController.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 5/11/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Cocoa

struct Movie: Codable {
    let id: Int
    let name: String
    let pg: String
    let genre: String
    let year: Int
    let duration: Int
}

extension Movie {
    enum CodingKeys: String, CodingKey {
        case id
        case name
        case pg = "movie_pg"
        case genre
        case year
        case duration
    }
}

class AdminViewController: NSViewController {
    
    @IBOutlet weak var salesTableView: NSTableView!
    
    @IBOutlet weak var moviesTableView: NSTableView!
    
    @IBOutlet weak var idTextFiled: NSTextField!
    @IBOutlet weak var nameTextField: NSTextField!
    @IBOutlet weak var pgTextField: NSTextField!
    @IBOutlet weak var genreTextField: NSTextField!
    @IBOutlet weak var yearTextField: NSTextField!
    @IBOutlet weak var durationTextField: NSTextField!
    
    @IBAction func confirmPressed(_ sender: Any) {
        let valid = idTextFiled.stringValue.isNotEmpty && nameTextField.stringValue.isNotEmpty &&
            pgTextField.stringValue.isNotEmpty && genreTextField.stringValue.isNotEmpty &&
            yearTextField.stringValue.isNotEmpty && durationTextField.stringValue.isNotEmpty
        guard valid else { return }
        Database.callVoidProcedure("edit_movie", with: [
            user.username,
            idTextFiled.stringValue,
            nameTextField.stringValue,
            pgTextField.stringValue,
            genreTextField.stringValue,
            yearTextField.stringValue,
            durationTextField.stringValue
        ]) { [weak self] error in
            guard let self = self else { return }
            guard let error = error else {
                self.dataSourcesReload()
                return
            }
            self.showAlert(title: error.error, content: error.error, buttonText: "OK", style: .critical)
        }
    }
    
    @IBAction func refreshAllPresed(_ sender: Any) {
        dataSourcesReload()
    }
    
    @IBAction func cancelOrderPressed(_ sender: Any) {
        guard sales.indices.contains(salesTableView.selectedRow) else { return }
        let saleID = sales[salesTableView.selectedRow].id
        Database.callVoidProcedure("cancel_order", with: [user.username, "\(saleID)"]) { [weak self] error in
            guard let self = self else { return }
            guard let error = error else {
                self.dataSourcesReload()
                return
            }
            self.showAlert(title: error.error, content: error.error, buttonText: "OK", style: .critical)
        }
    }
    
    private var sales = [Sale]() {
        didSet {
            salesTableView.reloadData()
        }
    }
    
    private var movies = [Movie]() {
        didSet {
            moviesTableView.reloadData()
        }
    }
    
    var user: User = .defaultUser(with: 0)
    
    private func dataSourcesReload() {
        Database.callProcedure("see_sales", with: [user.username]) {
            [weak self] (res: Result<[Sale], RequestError>) -> Void in
            guard let self = self else { return }
            switch res {
            case .success(let sales):
                self.sales = sales
            case .failure(let error):
                self.showAlert(title: "SALES LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
            }
        }
        Database.loadMovies { [weak self] res in
            guard let self = self else { return }
            switch res {
            case .success(let movies):
                self.movies = movies
            case .failure(let error):
                self.showAlert(title: "MOVIES LOAD ERROR", content: error.error, buttonText: "OK", style: .critical)
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        moviesTableView.delegate = self
        moviesTableView.dataSource = self
        salesTableView.delegate = self
        salesTableView.dataSource = self
        dataSourcesReload()
    }
    
}

extension AdminViewController {
    private enum MovieCellIdentifiers {
        static let id = "idCell"
        static let name = "nameCell"
        static let pg = "pgCell"
        static let genre = "genreCell"
        static let year = "yearCell"
        static let duration = "durationCell"
    }
    
    private enum SaleCellIdentifiers {
        static let id = "idCell"
        static let date = "dateCell"
        static let money = "moneyCell"
        static let user = "fromUserCell"
        static let card = "cardCell"
        static let cardValid = "cardValidCell"
        static let bank = "bankCell"
    }
}

extension AdminViewController: NSTableViewDataSource {
    func numberOfRows(in tableView: NSTableView) -> Int {
        if tableView == salesTableView {
            return sales.count
        } else {
            return movies.count
        }
    }
}

extension AdminViewController: NSTableViewDelegate {
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
           if tableView == salesTableView {
               return setupSaleCell(for: tableView, and: tableColumn, with: sales[row])
           } else if tableView == moviesTableView {
               return setupMovieCell(for: tableView, and: tableColumn, with: movies[row])
           } else {
               return nil
           }
       }
    
    func setupSaleCell(for tableView: NSTableView, and tableColumn: NSTableColumn?, with sale: Sale) -> NSView? {
        var text: String = ""
        var cellIdentifier: String = ""
        var cell: NSTableCellView
        
        if tableColumn == tableView.tableColumns[0] {
            text = "\(sale.id)"
            cellIdentifier = SaleCellIdentifiers.id
        } else if tableColumn == tableView.tableColumns[1] {
            text = "\(sale.date)"
            cellIdentifier = SaleCellIdentifiers.date
        } else if tableColumn == tableView.tableColumns[2] {
            text = "\(sale.moneyTransfered)"
            cellIdentifier = SaleCellIdentifiers.money
        } else if tableColumn == tableView.tableColumns[3] {
            text = "\(sale.user)"
            cellIdentifier = SaleCellIdentifiers.user
        } else if tableColumn == tableView.tableColumns[4] {
            text = "\(sale.card)"
            cellIdentifier = SaleCellIdentifiers.card
        } else if tableColumn == tableView.tableColumns[5] {
            text = "\(sale.cardValidDate)"
            cellIdentifier = SaleCellIdentifiers.cardValid
        } else if tableColumn == tableView.tableColumns[6] {
            text = "\(sale.bank)"
            cellIdentifier = SaleCellIdentifiers.bank
        } else {
            return nil
        }
        cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: self) as! NSTableCellView
        cell.textField?.stringValue = text
        return cell
    }
    
    func setupMovieCell(for tableView: NSTableView, and tableColumn: NSTableColumn?, with movie: Movie) -> NSView? {
        var text: String = ""
        var cellIdentifier: String = ""
        var cell: NSTableCellView
        
        if tableColumn == tableView.tableColumns[0] {
            text = "\(movie.id)"
            cellIdentifier = MovieCellIdentifiers.id
        } else if tableColumn == tableView.tableColumns[1] {
            text = "\(movie.name)"
            cellIdentifier = MovieCellIdentifiers.name
        } else if tableColumn == tableView.tableColumns[2] {
            text = "\(movie.pg)"
            cellIdentifier = MovieCellIdentifiers.pg
        } else if tableColumn == tableView.tableColumns[3] {
            text = "\(movie.genre)"
            cellIdentifier = MovieCellIdentifiers.genre
        } else if tableColumn == tableView.tableColumns[4] {
            text = "\(movie.year)"
            cellIdentifier = MovieCellIdentifiers.year
        } else if tableColumn == tableView.tableColumns[5] {
            text = "\(movie.duration)"
            cellIdentifier = MovieCellIdentifiers.duration
        } else {
            return nil
        }
        cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: self) as! NSTableCellView
        cell.textField?.stringValue = text
        return cell
    }
}

extension AdminViewController: Userable {}
