let winhook;

        winhook = require('./build/Debug/winhook.node');


var input = new winhook.Input();
input.mi = {
        dx: 50,
        dy: 50,
        mouseData: 5005,
        dwFlags: 6006,
        time: 15052525
};
input.ki = {
        wVk: 52,
        wScan: 5200,
        dwFlags: 683,
        time: 1252151,
}
console.log(input);
