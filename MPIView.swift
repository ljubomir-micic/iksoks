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
        VStack {
            TextField("IP Address", text: $ipa)
            
            Button {
                didConn = true
                show = false
            } label: {
                Text("Connect")
            }
        }
    }
}

//#Preview {
//    MPIView()
//}
