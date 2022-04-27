const { SerialPort } = require('serialport');
const MessageBuffer = require('./MessageBuffer');

class Controller {
    constructor(settings) {
        this.serialPortCom = new SerialPort({ path: settings.port, autoOpen: false, baudRate: parseInt(settings.baudrate) });
        this.closed = true;
        this.received = new MessageBuffer("]");

        this.cmi = [];
    }

    async _parseMessage(message){
        message = message.replace("[" , "").replace("]" , "");
        if ( message.includes("RVS") ){
            message = message.replace("RVS" , "");
            let _values = message.split(':');
            let cmiIdx = parseInt(_values[0]);

            this.cmi[cmiIdx] = {
                valid : parseInt(_values[1]) ? true : false,
                voltagemV : parseInt(_values[2]),
                voltagemVMin : parseInt(_values[3]),
                voltagemVMax : parseInt(_values[4]),
                internalTemp : parseInt(_values[5]),
                externalTemp : parseInt(_values[6]),
                inBypass : parseInt(_values[7]),
                PWMValue : parseInt(_values[8]),
                badPacketCount : parseInt(_values[9]),
                PacketReceivedCount : parseInt(_values[10]),
                BalanceCurrentCount : parseInt(_values[11]),
            }

            console.log(this.cmi);
        }
    }

    async open() {
        return new Promise((resolve, reject) => {
            if ( this.closed == false){
                resolve();
            } else {
                this.serialPortCom.open((err) => {
                    if (err) {
                        this.closed = true;
                        reject(err);
                        return;
                    }
                    this.closed = false;
                    this.serialPortCom.on('data', (chunk) => {
                        this.received.push(chunk);
                        while (!this.received.isFinished()) {
                            const message = this.received.handleData();
                            this._parseMessage(message + "]");
                        }
                    });
                    resolve();
                });
            }
        })
    }

    async close() {
        return new Promise((resolve, reject) => {
            if ( this.closed == true){
                resolve();
            } else {
                this.serialPortCom.close((err) => {
                    if (err) {
                        this.closed = false;
                        reject(err);
                        return;
                    }
                    this.closed = true;
                    resolve();
                });
            }
        });
    }

    async cmiReadVoltageAndStatus(idx){
        return new Promise((resolve, reject) => {
            if ( this.closed == true){
                reject();
            } else {
                this.serialPortCom.write("[RVS" + idx + "]");
            }
        });
    }
}

module.exports = Controller