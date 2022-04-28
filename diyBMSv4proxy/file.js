const fs = require('fs');
const path = require('path');

module.exports = {
    read: async function (path) {
        return new Promise((resolve, reject) => {
            let data = fs.readFileSync(path, 'utf8');
            resolve(data);
        });
    },
    save: async function (path, data) {
        return new Promise((resolve, reject) => {
            fs.writeFileSync(path, data, 'utf8');
            resolve();
        });
    },

    saveBankConfiguration: async function (data) {
        if (typeof data != "string") {
            data = JSON.stringify(data);
        }
        await this.save(path.resolve(__dirname, './bankconfig.json'), data);
    },
    readBankConfiguration: async function () {
        let obj = { totalControllers: 1, totalSeriesModules_0: 1, totalBanks_0: 1, baudrate_0: 9600 };
        try {
            let str = await this.read(path.resolve(__dirname, './bankconfig.json'));
            return JSON.parse(str);
        } catch (err) {
            return obj;
        }
    },

    saveGlobalSettings: async function (data) {
        if (typeof data != "string") {
            data = JSON.stringify(data);
        }
        await this.save(path.resolve(__dirname, './globalsettings.json'), data);
    },
    readGlobalSettings: async function () {
        let obj = { BypassOverTempShutdown_0: 65, BypassThresholdmV_0: 3000 };
        try {
            let str = await this.read(path.resolve(__dirname, './globalsettings.json'));
            return JSON.parse(str);
        } catch (err) {
            return obj;
        }
    },

    readRules: async function () {
        let obj = { timenow: new Date().getTime(), rules_0: [], relaydefault_0: [false, false, false,false] };
        for (let i = 0; i < 8; i++) {
            obj.rules_0.push({
                value: 0,
                hysteresis: 0,
                triggered: false,
                relays: [null, null, null, null]
            })
        }
        try {
            let str = await this.read(path.resolve(__dirname, './rules.json'));
            return JSON.parse(str);
        } catch (err) {
            return obj;
        }
    },

    saveRules: async function (data) {
        if (typeof data != "string") {
            data = JSON.stringify(data);
        }
        await this.save(path.resolve(__dirname, './rules.json'), data);
    },
    readSettings: async function () {
        let settings = {};
        try {
            let controllersSettings = await this.readBankConfiguration();
            let globalSettings = await this.readGlobalSettings();

            settings.totalControllers = controllersSettings.totalControllers;
            for (let i = 0; i < settings.totalControllers; i++) {
                settings["totalNumberOfSeriesModules_" + i] = controllersSettings["totalSeriesModules_" + i];
                settings["totalNumberOfBanks_" + i] = controllersSettings["totalBanks_" + i];
                settings["baudRate_" + i] = controllersSettings["baudrate_" + i];

                settings["BypassOverTempShutdown_" + i] = globalSettings["BypassOverTempShutdown_" + i];
                settings["BypassThresholdmV_" + i] = globalSettings["BypassThresholdmV_" + i];
            }

            settings.NTPServerName = "";
            settings.TimeZone = "";
            settings.MinutesTimeZone = "";
            settings.DST = "";

            settings.FreeHeap = 0;
            settings.MinFreeHeap = 0;
            settings.HeapSize = 0;
            settings.SdkVersion = 0;

            settings.HostName = "hello world";

            settings.now = new Date().getTime();

            return settings;
        } catch (err) {
            return {};
        }
    }
}