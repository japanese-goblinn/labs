//
//  UsersView.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/26/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import SwiftUI

struct ChooserView: View {
    
    let user: User
    
    var body: some View {
        switch user.role {
        case .user:
            return AnyView(
                VStack {
                    ViewControllerWrapper<SharedViewController>()
                }
                .frame(maxWidth: .infinity, maxHeight: .infinity)
            )
        case .admin:
            return AnyView(
                VStack {
                    ViewControllerWrapper<AdminViewController>()
                    ViewControllerWrapper<SharedViewController>()
                }
                .frame(maxWidth: .infinity, maxHeight: .infinity)
            )
        case .headmaster:
            return AnyView(
                VStack {
                    ViewControllerWrapper<HeadmasterViewController>()
                    ViewControllerWrapper<SharedViewController>()
                }
                .frame(maxWidth: .infinity, maxHeight: .infinity)
            )
        case .manager:
            return AnyView(
                VStack {
                    ViewControllerWrapper<ManagerViewController>()
                    ViewControllerWrapper<SharedViewController>()
                }
                .frame(maxWidth: .infinity, maxHeight: .infinity)
            )
        }
    }
}
