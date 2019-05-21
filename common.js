var winhook = require('./index');

function createVirtualKeyInput(wVk, dwFlags) {
    var input = new winhook.Input();
    input.type = winhook.Winhook.INPUT_KEYBOARD;
    if (dwFlags) {
        input.ki = {
            wVk: wVk,
            dwFlags: dwFlags
        };
    } else {
        input.ki = {
            wVk: wVk
        };
    }
    return input;
}

module.exports = {
    createVirtualKeyInput,
    isUpper: function (a) {
        return (a >= 'A' && a <= 'Z')
    }
}
