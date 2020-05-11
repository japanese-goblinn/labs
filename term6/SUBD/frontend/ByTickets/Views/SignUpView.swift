//
//  SignUpView.swift
//  ByTickets
//
//  Created by Kirill Gorbachyonok on 3/25/20.
//  Copyright © 2020 Kirill Gorbachyonok. All rights reserved.
//

import SwiftUI

struct CardInputView: View {
    
    @State private var number = ""
    @State private var bank = ""
    @State private var date = ""
    
    var body: some View {
        VStack(alignment: .leading) {
            Text("Credit card")
            HStack {
                TextField("Number", text: $number)
                TextField("Bank", text: $bank)
                TextField("Valid until (y-m-d)", text: $number)
            }
        }
        .padding(.top)
        .padding(.bottom)
    }
}

struct SignUpView: View {
    
    @State private var firstname = ""
    @State private var lastname = ""
    @State private var username = ""
    @State private var email = ""
    @State private var password = ""
    @State private var repeatPassword = ""
    @State private var selectedRole = 0
    
    @State private var showingAlert = false
    @State private var message = ""
    
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
                CardInputView()
                SecureField("Password", text: $password)
                SecureField("Repeat password", text: $repeatPassword)
                
                Button(action: {
                    let newUser = User(
                        id: 0,
                        firstname: self.firstname,
                        lastname: self.lastname,
                        username: self.username,
                        email: self.email,
                        role: User.Role.init(rawValue: self.roles[self.selectedRole])!,
                        banned: .no,
                        phone: nil,
                        contryCode: nil,
                        lastLogin: Date(),
                        registration: Date(),
                        cardID: nil,
                        passwordHashValue: self.password.hashValue
                    )
                    Database.insert(newUser) { res in
                        switch res {
                        case .failure(let error):
                            self.message = error.reason
                            self.showingAlert = true
                        case .success(_):
                            Database.authenticate(
                                username: self.username,
                                password: self.password
                            ) { res in
                                switch res {
                                case .failure(let error):
                                    self.message = error.reason
                                    self.showingAlert = true
                                case .success(let user):
                                    switch user.role {
                                    case .user:
                                        ChooserView(user: user)
                                            .openInNewWindow("User '\(user.username)'")
                                    case .admin:
                                        ChooserView(user: user)
                                            .openInNewWindow("Admin '\(user.username)'")
                                    case .headmaster:
                                        ChooserView(user: user)
                                            .openInNewWindow("Headmaster '\(user.username)'")
                                    case .manager:
                                        ChooserView(user: user)
                                            .openInNewWindow("Manager '\(user.username)'")
                                    }
                                }
                            }
                        }
                    }
                }) {
                    Text("Sign Up")
                }
                .disabled(!isValid)
                .alert(isPresented: $showingAlert) {
                    Alert(title: Text("Error"),
                          message: Text(message),
                          dismissButton: .default(Text("OK")))
                    
                }
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
