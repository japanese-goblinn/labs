//
//  ViewControllerWrapper.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/26/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation
import SwiftUI

protocol Userable: AnyObject {
    var user: User { get set }
}

struct ViewControllerWrapper<Controller: NSViewController & Userable>: NSViewControllerRepresentable {
    
    let user: User
    
    func makeNSViewController(context: RepresentableContext) -> Controller {
        let controller = Controller()
        controller.user = user
        return controller
    }
    func updateNSViewController(_ nsViewController: NSViewControllerType, context: RepresentableContext) {}
}

extension ViewControllerWrapper {
    typealias NSViewControllerType = Controller
    typealias RepresentableContext = NSViewControllerRepresentableContext<ViewControllerWrapper>
}

