bluetooth.onBluetoothConnected(function () {
    connected = 1
})
bluetooth.onBluetoothDisconnected(function () {
    connected = 0
})
input.onButtonPressed(Button.A, function () {
    bluetooth.uartWriteString("YES")
})
input.onButtonPressed(Button.B, function () {
    bluetooth.uartWriteString("NO")
})
input.onButtonPressed(Button.AB, function () {
    bluetooth.uartWriteLine("OK!")
})
let connected = 0
basic.showString("AVM")
while (1) {
    if (connected == 1) {
        let msg = bluetooth.uartReadUntil(serial.delimiters(Delimiters.Colon))
        basic.showString(msg+" ")
    } else {
        basic.pause(1000)
    }
}
