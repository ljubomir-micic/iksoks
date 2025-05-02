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
    @State var win: Int32 = 0
    @State var showMPSetup: Bool = false
    @State var didConn: Bool = false
    @State var isServ: Bool = false
    @State var ipa: String = ""
    
    func WhoWins() -> String {
        if (win == 1) {
            return "X";
        } else if (win == 2) {
            return "O";
        }
        return "";
    }
    
    func WhoWins() -> Color {
        if (win == 1) {
            return Color(#colorLiteral(red: 1, green: 0, blue: 0, alpha: 1));
        } else if (win == 2) {
            return Color(#colorLiteral(red: 0, green: 0, blue: 1, alpha: 1));
        }
        return Color(#colorLiteral(red: 0.4941176471, green: 0.4955747378, blue: 0.4955747378, alpha: 1));
    }
    
    var body: some View {
        if (scene == .menu) {
            VStack {
                HStack {
                    Image("iksoks")
                        .resizable()
                        .frame(width: 75, height: 75)
                    
                    Text("XksOks")
                        .font(.title)
                        .padding(.leading)
                }
                
                Button {
                    scene = .game
                } label: {
                    ZStack {
                        RoundedRectangle(cornerRadius: 25, style: .continuous)
                            .frame(maxWidth: .infinity, maxHeight: 55)
                            .foregroundStyle(Color(#colorLiteral(red: 0, green: 0, blue: 1, alpha: 1)))
                            .padding(25)
                        Text("Start")
                            .foregroundStyle(.white)
                    }
                }
            }
        } else if (win != 0) {
            VStack {
                Spacer()
                Text("Igrac \(WhoWins()) je pobedio.\nDODIRNI EKRAN ZA NOVU PARTIJU")
                    .multilineTextAlignment(.center)
                    .foregroundStyle(.white)
                Spacer()
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)
            .background(WhoWins())
            .onTapGesture {
                if (gameLogic.isMultiplayer()) {
                    gameLogic.deinitMultiplayer()
                }
                gameLogic.startGame()
                win = 0
            }
        } else {
            VStack {
                HStack {
                    Button {
                        if (gameLogic.isMultiplayer()) {
                            gameLogic.deinitMultiplayer()
                        }
                        gameLogic.startGame()
                        win = 0
                        tapNum = 0
                    } label: {
                        Image(systemName: "arrow.clockwise")
                    }
                    .disabled(tapNum==0)
                    
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
                            let x = Int32(location.y / cellSize)
                            let y = Int32(location.x / cellSize)
                            let res = gameLogic.handleInput(x, y)
                            if (gameLogic.isMultiplayer()) {
                                gameLogic.sendData("\(x)\(y)")
                            }
                            if (res == 101) { tapNum = 0; win = gameLogic.getValue(x, y) }
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
                        guard gameLogic.initMultiplayer(isServ, ipa) != 0 else {
                            isServ = false
                            didConn = false
                            return
                        }
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
