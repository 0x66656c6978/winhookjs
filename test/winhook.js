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
function zomg(payload, inputs, char, index) {
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

const notepadExecutable = "C:\\Windows\\System32\\notepad.exe";
const notepadWindowName = "Unbenannt - Editor";

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
        var WH = winhook.Winhook;

        var notepadHWND, notepadProcess;

        describe('GetLastError', function () {
            it('should return 0 if no error occured', function () {
                assert.equal(0, winhook.Winhook.GetLastError());
            })
        })

        describe('CreateProcess', function () {
            it('should create a process', function () {
                notepadProcess = WH.CreateProcess(notepadExecutable, null);
            })
        })

        describe('CloseHandle', function () {
            it('should close the process handle of the notepad process', function () {
                assert.equal(WH.CloseHandle(notepadProcess.hProcess), true);
            })
            it('should close the thread handle of the notepad process', function () {
                assert.equal(WH.CloseHandle(notepadProcess.hThread), true);
            })
        })

        describe('FindWindow', function () {
            it('should find notepad.exe', function () {
                notepadHWND = WH.FindWindow(notepadWindowName);
                assert.notEqual(notepadHWND, 0);
            });
        })

        describe('SetForegroundWindow', function () {
            it('should set notepad as foreground window', function () {
                assert.equal(WH.SetForegroundWindow(notepadHWND), true);
            })
        })

        describe('GetForegroundWindow', function () {
            it('should get the foreground window', function () {
                var notepadHWND2 = WH.GetForegroundWindow();
                assert.equal(notepadHWND, notepadHWND2);
            })
        })

        describe('SendInput', function () {
            it('should send Hello World to the notepad window', function () {
                const payload = `
Hello
World
`
                const inputs = payload.split('').reduce(zomg.bind(this, payload), []);
                var numInputsSent = WH.SendInput.apply(WH, inputs);
                assert.equal(numInputsSent, inputs.length);
            })


            it('should send ctrl+c to the notepad window', function () {
                var input = new winhook.Input();
                var inputs = [];
                input.type = winhook.Winhook.INPUT_KEYBOARD;
                input.ki = { wVk: 0x11 };  // ctrl
                inputs.push(input);
        
                input = new winhook.Input();
                input.type = winhook.Winhook.INPUT_KEYBOARD;
                input.ki = { wVk: 0x43 }; // C
                inputs.push(input);
        
                input = new winhook.Input();
                input.type = winhook.Winhook.INPUT_KEYBOARD;
                input.ki = { wVk: 0x43, dwFlags: winhook.Input.KEYEVENTF_KEYUP }; // C up
                inputs.push(input);
        
                input = new winhook.Input();
                input.type = winhook.Winhook.INPUT_KEYBOARD;
                input.ki = { wVk: 0x11, dwFlags: winhook.Input.KEYEVENTF_KEYUP }; // ctrl up
                inputs.push(input);
                var numInputsSent = WH.SendInput.apply(WH, inputs);
                assert.equal(numInputsSent, inputs.length);
            })

            // // jk
            // it('should send random stuff to the notepad window', function () {
            //     var payload = '';
            //     for (var i = 0; i < 10000; i++) {
            //         payload += String.fromCharCode(Math.random() * 255)
            //     }
            //     const inputs = payload.split('').reduce(zomg.bind(this, payload), []);
            //     var numInputsSent = WH.SendInput.apply(WH, inputs);
            //     assert.equal(numInputsSent, inputs.length);
            // })

        });

        describe('CloseWindow', function () {
            it('should close the notepad window', function () {
                assert.equal(WH.CloseWindow(notepadHWND), true);
                assert.notEqual(WH.FindWindow(notepadWindowName), 0);
            })
        })

        describe('ShowWindow', function () {
            it('should maximize the notepad window', function () {
                assert.equal(WH.ShowWindow(notepadHWND, 3), false); // false means it was previously hidden, which it should be because of CloseWindow
            })
        })
    })
});
