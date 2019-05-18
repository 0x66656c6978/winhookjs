var assert = require('assert');
var winhook = require('../index');

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
});
