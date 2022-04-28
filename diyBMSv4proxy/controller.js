const { SerialPort } = require('serialport');
const MessageBuffer = require('./MessageBuffer');

class Controller {
    constructor(settings) {
        this.serialPortCom = new SerialPort({ path: settings?.port || "COM1", autoOpen: true, baudRate: parseInt(settings?.baudrate ? settings?.baudrate : 9600) });
        this.closed = true;
        this.received = new MessageBuffer("]");

        this.totalSeriesModules = parseInt(settings.totalSeriesModules);
        this.totalBanks = parseInt(settings.totalBanks);
        this.BypassOverTempShutdown = parseInt(settings.BypassOverTempShutdown);
        this.BypassThresholdmV = parseInt(settings.BypassThresholdmV);

        this.cmi = [];

        this.serialPortCom.on('data', (chunk) => {
            this.received.push(chunk);
            while (!this.received.isFinished()) {
                const message = this.received.handleData();
                this._parseMessage(message + "]");
            }
        });

        this.serialPortCom.on('open', (err) => {
            this.closed = false;
        });
    }

    async _parseMessage(message) {
        message = message.replace("[", "").replace("]", "");
        if (message.includes("RVS")) {
            message = message.replace("RVS", "");
            let _values = message.split(':');
            let cmiIdx = parseInt(_values[0]);

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
            }
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
        return new Promise((resolve, reject) => {
            let cpt = 0;
            let __it = setInterval(() => {
                cpt++;
                if (this.closed == false) {
                    clearInterval(__it);
                    resolve();
                    return;
                }

                if (cpt >= 30) {
                    clearInterval(__it);
                    reject();
                    return;
                }
            }, 1000);
        })
    }

    async close() {
        return new Promise((resolve, reject) => {
            if (this.closed == true) {
                resolve();
            } else {
                this.serialPortCom.close((err) => {
                    if (err) {
                        this.closed = false;
                        reject(err);
                        return;
                    }
                    clearInterval(this.interval);
                    this.closed = true;
                    resolve();
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
        return new Promise((resolve, reject) => {
            if (this.closed == true) {
                reject();
            } else {
                this.serialPortCom.write("[WCS255:" + this.totalBanks + ":" + this.totalSeriesModules + ":" + this.BypassOverTempShutdown + ":" + this.BypassThresholdmV + "]");
                resolve();
            }
        });
    }

    async controllerSetCmiIdentify(cmiIdx) {
        return new Promise((resolve, reject) => {
            if (this.closed == true) {
                reject();
            } else {
                this.serialPortCom.write("[ID" + cmiIdx + "]");
                resolve();
            }
        });
    }
}

module.exports = Controller