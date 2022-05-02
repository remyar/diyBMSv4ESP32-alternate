const express = require('express');
var cors = require('cors');
var multer = require('multer');
var upload = multer();
var file = require('./file');
var Controller = require('./controller');
const app = express();
const { SerialPort } = require('serialport');

global.Controllers = [];

async function scanAllControllers(){
    let _ports = [];
    if ( process.platform === "win32" ){
        _ports = (await SerialPort.list()).filter((el) => el.friendlyName.includes("USB-SERIAL CH340"));
    } else {
        _ports.push({ path : "/dev/ttyUSB0" });
    }

    global.Controllers = [];
    
    for (let i = 0; i < _ports.length; i++) {
        let settings = {
            baudrate : 9600,
            port : _ports[i].path
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

    await scanAllControllers();

  //  loadControllerSettings();
});