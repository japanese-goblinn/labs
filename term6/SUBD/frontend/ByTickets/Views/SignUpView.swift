//
//  SignUpView.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import SwiftUI

struct SignUpView: View {
    
    @State private var firstname = ""
    @State private var lastname = ""
    @State private var username = ""
    @State private var email = ""
    @State private var password = ""
    @State private var repeatPassword = ""
    @State private var selectedRole = 0
    
    private var isValid: Bool {
        firstname.isNotEmpty && lastname.isNotEmpty && username.isNotEmpty && email.isNotEmpty && password.isNotEmpty && repeatPassword.isNotEmpty && repeatPassword == password
    }
    
    private let roles = User.Role.allCases.map { "\($0)" }
    
    var body: some View {
        VStack(alignment: .leading) {
            Text("Introduce yourself")
                .font(.subheadline)
                .fontWeight(.bold)
            Divider()
            VStack(alignment: .leading) {
                TextField("First name", text: $firstname)
                TextField("Last name", text: $lastname)
                Picker(selection: $selectedRole, label: Text("Role:")) {
                    ForEach(0 ..< roles.count) {
                        Text(self.roles[$0])
                    }
                }
                TextField("Username", text: $username)
                TextField("Email", text: $email)
                SecureField("Password", text: $password)
                SecureField("Repeat password", text: $repeatPassword)
                
                Button(action: {
                    Text("sdfs")
                }) {
                    Text("Sign Up")
                }
                .disabled(!isValid)
            }
            .padding()
        }
        .padding()
    }
}

struct SignUpView_Previews: PreviewProvider {
    static var previews: some View {
        SignUpView()
    }
}
