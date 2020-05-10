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
    
    @State private var message = ""
    
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
                    Database.authenticate(username: self.username, password: self.password) { res in
                        switch res {
                        case .failure(let error):
                            self.message = error.reason
                            self.showingAlert = true
                        case .success(let user):
                            UsersView()
                                .openInNewWindow("Users")
                        }
                    }
                }) {
                    Text("Sign In")
                }
                .disabled(username.isEmpty || password.isEmpty)
                .alert(isPresented: $showingAlert) {
                    Alert(title: Text("Error"),
                          message: Text(message),
                          dismissButton: .default(Text("OK")))
                }
            }
            .padding()
//
//            Button(action: {
//                UsersView()
//                    .openInNewWindow("Users")
//            }) {
//                Text("Test")
//            }
        }
        .padding()
    }
}

struct SignInView_Previews: PreviewProvider {
    static var previews: some View {
        SignInView()
    }
}
