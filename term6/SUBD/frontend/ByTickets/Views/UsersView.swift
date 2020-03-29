//
//  UsersView.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/26/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import SwiftUI

struct UsersView: View {
    var body: some View {
        ViewControllerWrapper<UsersViewController>()
            .frame(maxWidth: .infinity, maxHeight: .infinity)
    }
}

struct UsersView_Previews: PreviewProvider {
    static var previews: some View {
        UsersView()
    }
}
