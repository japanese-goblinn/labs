//
//  ViewControllerWrapper.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/26/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import Foundation
import SwiftUI

struct ViewControllerWrapper<Controller: NSViewController>: NSViewControllerRepresentable {
    func makeNSViewController(context: RepresentableContext) -> Controller { Controller() }
    func updateNSViewController(_ nsViewController: NSViewControllerType, context: RepresentableContext) {}
}

extension ViewControllerWrapper {
    typealias NSViewControllerType = Controller
    typealias RepresentableContext = NSViewControllerRepresentableContext<ViewControllerWrapper>
}

