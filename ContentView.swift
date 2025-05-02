//
//  ContentView.swift
//  iksoks
//
//  Created by Љубомир Мићић on 30. 4. 2025..
//

import SwiftUI

enum scene {
    case menu
    case game
}

struct ContentView: View {
    var gameLogic: LogicHelper = LogicHelper()
    @State var scene: scene = .menu
    @State var tapNum: Int = 0
    @State var won: Bool = false
    @State var showMPSetup: Bool = false
    @State var didConn: Bool = false
    @State var isServ: Bool = false
    @State var ipa: String = ""
    
    var body: some View {
        if (scene == .menu) {
            VStack {
                Button {
                    scene = .game
                } label: {
                    ZStack {
                        RoundedRectangle(cornerRadius: 25, style: .continuous)
                            .frame(maxWidth: .infinity, maxHeight: 75)
                            .foregroundStyle(Color(#colorLiteral(red: 0, green: 0, blue: 1, alpha: 1)))
                            .padding(25)
                        Text("Start")
                            .foregroundStyle(.white)
                    }
                }
            }
        } else if (won) {
            VStack {
                Spacer()
                Text("Game ended. Player won. Tap to play again.")
                    .foregroundStyle(.white)
                Spacer()
            }
            .background(Color(#colorLiteral(red: 0.0168418996, green: 0.198341459, blue: 1, alpha: 1)))
            .frame(maxWidth: .infinity, maxHeight: .infinity)
            .onTapGesture {
                gameLogic.startGame()
                won = false
            }
        } else {
            VStack {
                HStack {
                    Button {
                        
                    } label: {
                        Image(systemName: "gearshape.fill")
                    }
                    
                    Spacer()
                    
                    Button {
                        showMPSetup = true
                    } label: {
                        Image(systemName: "person.2.fill")
                    }
                }
                .padding()
                Spacer()
                GeometryReader { proxy in
                    let size = min(proxy.size.width, proxy.size.height)
                    let cellSize = (size / 3)
                    
                    ZStack {
                        ForEach(1 ..< 3) { i in
                            Path { path in
                                let x = CGFloat(i) * cellSize
                                path.move(to: CGPoint(x: x, y: 0))
                                path.addLine(to: CGPoint(x: x, y: size))
                            }
                            .stroke(Color.accentColor, lineWidth: 2)
                        }
                        
                        ForEach(1 ..< 3) { i in
                            Path { path in
                                let y = CGFloat(i) * cellSize
                                path.move(to: CGPoint(x: 0, y: y))
                                path.addLine(to: CGPoint(x: size, y: y))
                            }
                            .stroke(Color.accentColor, lineWidth: 2)
                        }
                        
                        if (tapNum > -1) {
                            ForEach(0..<3, id: \.self) { row in
                                ForEach(0..<3, id: \.self) { col in
                                    let value = gameLogic.getValue(Int32(row), Int32(col))
                                    let symbol = value == 1 ? "X" : (value == 2 ? "O" : "")
                                    let ntv = Int(gameLogic.nextToVanish())
                                    let isNTV = (ntv&3)==col && (ntv>>2)==row
                                    
                                    if !symbol.isEmpty {
                                        Text(symbol)
                                            .font(.system(size: cellSize * 0.7))
                                            .opacity(isNTV ? 0.5 : 1)
                                            .frame(width: cellSize, height: cellSize)
                                            .position(x: CGFloat(col) * cellSize + cellSize / 2,
                                                      y: CGFloat(row) * cellSize + cellSize / 2)
                                    }
                                }
                            }
                        }
                    }
                    .frame(width: size, height: size)
                }
                .aspectRatio(1, contentMode: .fit)
                .padding()
                .contentShape(Rectangle())
                .simultaneousGesture(
                    DragGesture(minimumDistance: 0)
                        .onEnded { value in
                            let location = value.location
                            let size = min(UIScreen.main.bounds.width, UIScreen.main.bounds.height) // Or whatever actual size you want
                            let cellSize = size / 3
                            let res = gameLogic.handleInput(Int32(location.y / cellSize), Int32(location.x / cellSize))
                            if (res == 101) { tapNum = 0; won = true }
                            else { tapNum += 1 }
                        }
                )
                Spacer()
            }
            .sheet(isPresented: $showMPSetup) {
                MPIView(show: $showMPSetup, didConn: $didConn, isServ: $isServ, ipa: $ipa)
            }
            .onChange(of: showMPSetup) { oldValue, newValue in
                if (newValue == false) {
                    if didConn == true {
                        gameLogic.initMultiplayer(isServ, ipa)
                    } else {
                        isServ = false
                    }
                }
            }
        }
    }
}

#Preview {
    ContentView()
}
