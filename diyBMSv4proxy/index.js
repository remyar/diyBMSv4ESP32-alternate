const express = require('express');
var cors = require('cors');
var multer = require('multer');
var upload = multer();
var file = require('./file');
var Controller = require('./controller');
var logging = require('./logging');
const app = express();
const { SerialPort } = require('serialport');

global.Controllers = [];
global.logging = {
    loggingFrequencySeconds: 15,
    loggingEnabled: false
};

let cptlogging = 0;
global.loggingTask = setInterval(() => {
    cptlogging++;
    if ((cptlogging % global.logging.loggingFrequencySeconds) == 0) {
        if (global.logging.loggingEnabled) {
            let _date = new Date().toLocaleDateString("fr").replace("/","_").replace("/","_");
            let loggingFileName = "./data_" + _date + ".csv";
            logging.process(loggingFileName);
        }
    }
}, 1000);

async function scanAllControllers() {
    let _ports = [];
    if (process.platform === "win32") {
        _ports = (await SerialPort.list()).filter((el) => el.friendlyName.includes("USB-SERIAL CH340"));
    } else {
        _ports.push({ path: "/dev/ttyUSB0" });
    }

    global.Controllers = [];

    for (let i = 0; i < _ports.length; i++) {
        let settings = {
            baudrate: 9600,
            port: _ports[i].path
        }
        global.Controllers.push(new Controller(settings));
    }

    for (let i = 0; i < global.Controllers.length; i++) {
        global.Controllers[i].open();
    }

}

app.use(express.json())    // <==== parse request body as JSON
app.options('*', cors()) // include before other routes

app.use(upload.array());

app.use('/', cors(), require('./routes'));

app.listen(3005, async () => {
    console.log("Serveur à l'écoute");
    global.logging = await file.readGlobalSettings();
    await scanAllControllers();

});