//
//  ContentView.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/16/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        TabView {
            SignInView()
                .tabItem {
                    Text("Sign In")
                }
                .tag(0)
        
            SignUpView()
                .tabItem {
                    Text("Sign Up")
                }
                .tag(1)
        }
        .padding()
    }
}


struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
