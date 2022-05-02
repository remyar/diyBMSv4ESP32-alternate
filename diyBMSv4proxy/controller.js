const { SerialPort } = require('serialport');
const MessageBuffer = require('./MessageBuffer');

class Controller {
    constructor(settings) {

        this.serialPortCom = new SerialPort({ path: settings?.port || "COM1", autoOpen: true, baudRate: parseInt(settings?.baudrate ? settings?.baudrate : 9600) });
        this.received = new MessageBuffer("]");

        this.totalSeriesModules = parseInt(settings.totalSeriesModules);
        this.totalBanks = parseInt(settings.totalBanks);
        this.BypassOverTempShutdown = parseInt(settings.BypassOverTempShutdown) || 65;
        this.BypassThresholdmV = parseInt(settings.BypassThresholdmV) || 4000;

        this.cmi = [];
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
    }

    async _parseMessage(message) {
        message = message.replace("[", "").replace("]", "");
        if (message.includes("RVS")) {
            message = message.replace("RVS", "");
            let _values = message.split(':');
            let cmiIdx = parseInt(_values[0]);
            if ( cmiIdx == NaN || (cmiIdx > (this.totalSeriesModules * this.totalBanks))){
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
        }
        if (message.includes("RCVS")) {
            message = message.replace("RCVS", "");
            let _values = message.split(':');
            this.rule_outcome = [..._values];
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
        return new Promise( (resolve, reject) => {
            let cpt = 0;
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
            }, 100);
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
    async controllerSetSettings() {
        return new Promise(async (resolve, reject) => {
            if (this.serialPortCom.isOpen == false) {
                reject();
            } else {
                this.serialPortCom.write("[WCS255:" + this.totalBanks + ":" + this.totalSeriesModules + ":" + this.BypassOverTempShutdown + ":" + this.BypassThresholdmV + "]");
                resolve();
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

    async controllerSetRules(rules) {
        return new Promise(async (resolve, reject) => {
            if (this.serialPortCom.isOpen == false) {
                reject();
            } else {

                for (let i = 0; i < rules.rules.length; i++) {
                    let _r = rules.rules[i];
                    let str = ("[WCR" + i + ":" + _r.value + ":" + _r.hysteresis + ":");

                    for (let j = 0; j < 4; j++) {
                        let val = 0
                        if (_r.relays[j] == 'On') {
                            val = 0xFF;
                        } else if (_r.relays[j] == 'Off') {
                            val = 0x99;
                        }
                        str += ("" + val + "");
                        str += (":");
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
                    this.serialPortCom.write(":");
                }
                this.serialPortCom.write("]");

                resolve();
            }
        });
    }
}

module.exports = Controller