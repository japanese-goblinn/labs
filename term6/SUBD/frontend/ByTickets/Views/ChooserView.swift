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
                ViewControllerWrapper<SharedViewController>()
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
            )
        case .admin:
            return AnyView(
                ViewControllerWrapper<AdminViewController>()
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
            )
        case .headmaster:
            return AnyView(
                ViewControllerWrapper<HeadmasterViewController>()
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
            )
        case .manager:
            return AnyView(
                ViewControllerWrapper<ManagerViewController>()
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
            )
        }
    }
}
