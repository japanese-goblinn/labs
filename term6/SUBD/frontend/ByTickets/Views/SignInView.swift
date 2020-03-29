//
//  SignInView.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import SwiftUI

struct SignInView: View {
    
    @State private var username = ""
    @State private var password = ""
    @State private var showingAlert = false
    
    var body: some View {
        VStack(alignment: .leading) {
            Text("Welcome back")
                .font(.subheadline)
                .fontWeight(.bold)
            Divider()
            
            VStack(alignment: .leading) {
                TextField("Username", text: $username)
                SecureField("Password", text: $password)
                Button(action: {
                    Database.fetchUsers { result in
                        proceed(result)
                        self.showingAlert = true
                    }
                }) {
                    Text("Sign In")
                }
                .disabled(username.isEmpty || password.isEmpty)
                .alert(isPresented: $showingAlert) {
                    Alert(title: Text("Important message"),
                          message: Text("Wear sunscreen"),
                          dismissButton: .default(Text("OK")))
                }
            }
            .padding()
            
            Button(action: {
                (NSApplication.shared.delegate as! AppDelegate).window.close()
                UsersView()
                    .openInNewWindow("Users")
            }) {
                Text("Test")
            }
        }
        .padding()
    }
}

struct SignInView_Previews: PreviewProvider {
    static var previews: some View {
        SignInView()
    }
}
