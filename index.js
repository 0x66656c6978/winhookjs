let winhook;

try {
    winhook = require('./build/Release/winhook.node');
} catch(ex) {
    try {
        winhook = require('./build/Debug/winhook.node');
    } catch(ex2) {
        throw ex;
    }
}

var input = new winhook.Input();
input.type = 5;

module.exports = winhook;