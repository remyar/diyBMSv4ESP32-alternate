const { SerialPort } = require('serialport');
const MessageBuffer = require('./MessageBuffer');

class Controller {
    constructor(settings) {

        this.serialPortCom = new SerialPort({ path: settings?.port || "COM1", autoOpen: false, baudRate: parseInt(settings?.baudrate ? settings?.baudrate : 9600) });
        this.received = new MessageBuffer("]");

        this.totalSeriesModules = 0;
        this.totalBanks = 0;
        this.BypassOverTempShutdown = 65;
        this.BypassThresholdmV = 4000;

        this.cmi = [];
        this.rules = [];
        this.relayDefault = [];
        this.rule_outcome = [];

        this.serialPortCom.on('data', (chunk) => {
            this.received.push(chunk);
            while (!this.received.isFinished()) {
                const message = this.received.handleData();
                this._parseMessage(message + "]");
            }
        });

        this.serialPortCom.on('open', (err) => { });

        this.serialPortCom.on('error', (err) => {
            console.log(err);
        });

        this.RELAY_RULES = 10;
        this.RELAY_TOTAL = 3;
    }

    async _parseMessage(message) {
        message = message.replace("[", "").replace("]", "");
        if (message.includes("RVS")) {
            message = message.replace("RVS", "");
            let _values = message.split(':');
            let cmiIdx = parseInt(_values[0]);
            if (cmiIdx == NaN || (cmiIdx > (this.totalSeriesModules * this.totalBanks))) {
                return;
            }
            this.cmi[cmiIdx] = {
                valid: parseInt(_values[1]) ? true : false,
                voltagemV: parseInt(_values[2]),
                voltagemVMin: parseInt(_values[3]),
                voltagemVMax: parseInt(_values[4]),
                internalTemp: parseInt(_values[5]),
                externalTemp: parseInt(_values[6]),
                inBypass: parseInt(_values[7]),
                PWMValue: parseInt(_values[8]),
                badPacketCount: parseInt(_values[9]),
                PacketReceivedCount: parseInt(_values[10]),
                BalanceCurrentCount: parseInt(_values[11]),
            };
        } else if (message.includes("RCVS")) {
            message = message.replace("RCVS", "");
            let _values = message.split(':');
            this.rule_outcome = [..._values];
        } else if (message.includes("RCONF")) {
            message = message.replace("RCONF", "");
            let _values = message.split(':');

            this.totalBanks = parseInt(_values[0]);
            this.totalSeriesModules = parseInt(_values[1]);
            this.BypassOverTempShutdown = parseInt(_values[2]);
            this.BypassThresholdmV = parseInt(_values[3]);
        } else if (message.includes("RRULES")) {
            message = message.replace("RRULES", "");
            let _values = message.split(':');
            let _valueIdx = 0;

            for (let i = 0; i < this.RELAY_RULES; i++) {
                let __r = {};
                __r.rulevalue = parseInt(_values[_valueIdx]) || 0;
                _valueIdx++;
                __r.rulehysteresis = parseInt(_values[_valueIdx]) || 0;
                _valueIdx++;
                __r.rulerelaystate = [];
                for (let y = 0; y < this.RELAY_TOTAL; y++) {
                    __r.rulerelaystate[y] = parseInt(_values[_valueIdx]) || 0;
                    _valueIdx++;
                }
                this.rules[i] = __r;
            }

            for (let y = 0; y < this.RELAY_TOTAL; y++) {
                this.relayDefault[y] = parseInt(_values[_valueIdx]) || 0;
                _valueIdx++;
            }

        } else if (message.includes("GTIME")) {
            let now = new Date();
            let hours = now.getHours() * 60;
            let minutes = now.getMinutes();
            let secSinceMidnight = hours + minutes;
            this.serialPortCom.write("[WTIME" + secSinceMidnight + "]");
        }
    }
    /*
        async _cmiReadVoltageAndStatus(){
            for (let i = 0; i < this.totalSeriesModules; i++) {
                await c.cmiReadVoltageAndStatus(i);
            }
        }
    */
    async open() {
        return new Promise(async (resolve, reject) => {
            if (this.serialPortCom.isOpen == true) {
                resolve();
            } else {
                try {
                    await this.serialPortCom.open();
                    await this.sleep(3000);
                } catch (err) {
                    console.error(err);
                    reject(err);
                }
            }

            /* let cpt = 0;
             let __it = setInterval(async () => {
                 cpt++;
                 if (this.serialPortCom.isOpen == false) {
                     this.serialPortCom.open();
                 }
                 if (this.serialPortCom.isOpen == true) {
                     clearInterval(__it);
                     await this.sleep(3000);
                     resolve();
                     return;
                 }
 
                 if (cpt >= 300) {
                     clearInterval(__it);
                     reject();
                     return;
                 }
             }, 100);*/
        })
    }

    async close() {
        return new Promise((resolve, reject) => {
            if (this.serialPortCom.isOpen == false) {
                resolve();
            } else {
                this.serialPortCom.close((err) => {
                    if (err) {
                        reject(err);
                        return;
                    }
                    setTimeout(() => {
                        resolve();
                    }, 5000)

                });
            }
        });
    }
    /*
        async cmiReadVoltageAndStatus(idx) {
            return new Promise((resolve, reject) => {
                if (this.closed == true) {
                    reject();
                } else {
                    this.serialPortCom.write("[RVS" + idx + "]");
                    resolve();
                }
            });
        }
    */
    async controllerSetSettings(settings) {
        return new Promise(async (resolve, reject) => {
            try {
                if (this.serialPortCom.isOpen == false) {
                    await this.open();
                }

                this.totalSeriesModules = parseInt(settings.totalSeriesModules) || 1;
                this.totalBanks = parseInt(settings.totalBanks) || 1;
                this.BypassOverTempShutdown = parseInt(settings.BypassOverTempShutdown) || 65;
                this.BypassThresholdmV = parseInt(settings.BypassThresholdmV) || 4000;

                this.serialPortCom.write("[WCS255:" + this.totalBanks + ":" + this.totalSeriesModules + ":" + this.BypassOverTempShutdown + ":" + this.BypassThresholdmV + "]");
                resolve();
            } catch (err) {
                reject(err);
            }
        });
    }

    async controllerSetCmiIdentify(cmiIdx) {
        return new Promise((resolve, reject) => {
            if (this.serialPortCom.isOpen == false) {
                reject();
            } else {
                this.serialPortCom.write("[ID" + cmiIdx + "]");
                resolve();
            }
        });
    }

    async sleep(delay) {
        return new Promise((resolve, reject) => {
            setTimeout(() => {
                resolve();
            }, delay)
        });
    }
    async controllerReadConf() {
        return new Promise(async (resolve, reject) => {
            try {
                if (this.serialPortCom.isOpen == false) {
                    await this.open();
                }
                await this.serialPortCom.write("[RCONF]");
                //     await this.sleep(200);
                resolve({
                    totalBanks: this.totalBanks,
                    totalSeriesModules: this.totalSeriesModules,
                    BypassOverTempShutdown: this.BypassOverTempShutdown,
                    BypassThresholdmV: this.BypassThresholdmV,
                })
            } catch (err) {
                reject(err);
            }
        });
    }

    async controllerGetRules() {
        return new Promise(async (resolve, reject) => {
            try {
                if (this.serialPortCom.isOpen == false) {
                    await this.open();
                }
                await this.serialPortCom.write("[RRULES]");
                resolve(this.rules);
            } catch (err) {
                reject(err);
            }
        });
    }

    async controllerSetRules(rules) {
        return new Promise(async (resolve, reject) => {
            try {
                if (this.serialPortCom.isOpen == false) {
                    await this.open();
                }

                for (let i = 0; i < this.RELAY_RULES; i++) {
                    let _r = rules.rules[i];
                    let str = ("[WCR" + i + ":" + _r.value + ":" + _r.hysteresis + ":");

                    for (let j = 0; j < this.RELAY_TOTAL; j++) {
                        let val = 0
                        if (_r.relays[j] == 'On') {
                            val = 0xFF;
                        } else if (_r.relays[j] == 'Off') {
                            val = 0x99;
                        }
                        str += ("" + val + "");
                        if ( j < (this.RELAY_TOTAL -1)){
                            str += (":");
                        }
                    }
                    str += "]";
                    this.serialPortCom.write(str);
                    await this.sleep(100);
                }

                this.serialPortCom.write("[WRS");
                for (let i = 0; i < rules.relaydefault.length; i++) {
                    let val = 0x99;
                    if (rules.relaydefault[i] == 'On') {
                        val = 0xFF;
                    }
                    this.serialPortCom.write("" + val + "");
                    if ( i < (rules.relaydefault.length -1)){
                        this.serialPortCom.write(":");
                    }
                }
                this.serialPortCom.write("]");
                await this.sleep(100);
                await this.serialPortCom.write("[RRULES]");
                await this.sleep(200);
                resolve();

            } catch (err) {
                reject(err);
            }
        });
    }
}

module.exports = Controller