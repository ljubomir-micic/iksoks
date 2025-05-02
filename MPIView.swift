//
//  MPIView.swift
//  iksoks
//
//  Created by Љубомир Мићић on 2. 5. 2025..
//

import SwiftUI

struct MPIView: View {
    @Binding var show: Bool
    @Binding var didConn: Bool
    @Binding var isServ: Bool
    @Binding var ipa: String
    
    var body: some View {
        HStack {
            TextField("IP Address", text: $ipa)
                .padding(5)
                .cornerRadius(15)
                .border(.accent)
            
            Button {
                didConn = true
                show = false
            } label: {
                Text("Connect")
            }
            .buttonStyle(.borderedProminent)
        }
        .padding()
    }
}

//#Preview {
//    MPIView()
//}
