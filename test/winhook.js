var assert = require('assert');
var winhook = require('../index');
var common = require('../common');

function canGetAndSetInteger(scopeName, valueName, isShortValue, isUnsigned) {
    return function () {
        var value = 0xFFFFFFFF;
        if (!isUnsigned) value = 0x0FFFFFFF;
        var input = new winhook.Input();
        var payload = {};
        assert.equal(input[scopeName][valueName], 0);
        payload[valueName] = value;
        input[scopeName] = payload;
        var changedValue = input[scopeName][valueName];
        if (isShortValue) {
            var cappedValue = value & ((1 << 16) - 1);
            assert.equal(cappedValue, changedValue);
        } else {
            assert.equal(value, changedValue);
        }
    }
}
function zomg(inputs, char, index) {
    var vk = char.charCodeAt(0);
    if (common.isUpper(char) && !common.isUpper(payload[index-1])) {
        inputs.push(common.createVirtualKeyInput(0x10, 0));
    }
    if (char.match(/[a-z]/)) {
        vk -= 0x20;
    }
    if (char == '\n') vk = 0x0D;
    inputs.push(common.createVirtualKeyInput(vk, 0));
    inputs.push(common.createVirtualKeyInput(vk, winhook.Input.KEYEVENTF_KEYUP));
    if (common.isUpper(char) && !common.isUpper(payload[index+1])) {
        inputs.push(common.createVirtualKeyInput(0x10, winhook.Input.KEYEVENTF_KEYUP));                        
    }
    return inputs;
}
describe('winhook', function () {
    describe('Input', function() {
        describe('#hi', function () {
            it('should set and get the uMsg property', canGetAndSetInteger('hi', 'uMsg', false, true));
            it('should set and get the wParamH property', canGetAndSetInteger('hi', 'wParamH', true, true));
            it('should set and get the wParamL property', canGetAndSetInteger('hi', 'wParamL', true, true));
        });
        describe('#mi', function () {
            it('should set and get the dx property', canGetAndSetInteger('mi', 'dx', false, false));
            it('should set and get the dy property', canGetAndSetInteger('mi', 'dy', false, false));
            it('should set and get the mouseData property', canGetAndSetInteger('mi', 'mouseData', false, true));
            it('should set and get the dwFlags property', canGetAndSetInteger('mi', 'dwFlags', false, true));
            it('should set and get the time property', canGetAndSetInteger('mi', 'time', false, true));
        })
        describe('#ki', function () {
            it('should set and get the wVk property', canGetAndSetInteger('ki', 'wVk', true, true));
            it('should set and get the wScan property', canGetAndSetInteger('ki', 'wScan', true, true));
            it('should set and get the dwFlags property', canGetAndSetInteger('ki', 'dwFlags', false, true));
            it('should set and get the time property', canGetAndSetInteger('ki', 'time', false, true));
        })
    });
    describe('Winhook', function() {
        var WH = new winhook.Winhook();
        describe('GetLastError', function () {
            it('should return 0 if no error occured', function () {
                assert.equal(0, winhook.Winhook.GetLastError());
            })
        })

        describe('CreateProcess', function () {
            it('should create a process', function () {
                WH.CreateProcess("C:\\Windows\\System32\\notepad.exe", null);
            })
        })

        describe('FindWindow', function () {
            it('should find notepad.exe', function () {
                var idx = WH.FindWindow("Unbenannt - Editor");
                assert.equal(idx, 0);
            });
        })

        describe('SetForegroundWindow', function () {
            it('should set notepad as foreground window', function () {
                var r = WH.SetForegroundWindow(0);
                assert.equal(r, true);
            })
        })

        describe('GetForegroundWindow', function () {
            it('should get the foreground window', function () {
                var idx = WH.GetForegroundWindow();
                assert.equal(idx, 0);
            })
        })

        describe('SendInput', function () {
            it('should send Hello World to the notepad window', function () {
                const payload = `
Hello
World
`
                const inputs = payload.split('').reduce(zomg, []);
                var numInputsSent = WH.SendInput.apply(WH, inputs);
                assert.equal(numInputsSent, inputs.length);
            })
            it('should send random stuff to the notepad window', function () {
                var payload = '';
                for (var i = 0; i < 10000; i++) {
                    payload += String.fromCharCode(Math.random() * 255)
                }
                const inputs = payload.split('').reduce(zomg, []);
                var numInputsSent = WH.SendInput.apply(WH, inputs);
                assert.equal(numInputsSent, inputs.length);
            })
        });

    })
});
