const express = require('express');
var cors = require('cors');
var multer = require('multer');
var upload = multer();
var file = require('./file');
var Controller = require('./Controller');
const app = express();

global.Controllers = [];


async function loadControllerSettings(){
    let _sett = await file.readBankConfiguration();
    global.Controllers = [];
    for ( let i = 0 ; i < _sett.totalControllers ; i++){
        let settings = {
            baudrate : _sett["baudrate_"+i],
            totalBanks : _sett["totalBanks_"+i],
            totalSeriesModules : _sett["totalSeriesModules_"+i],
            port : _sett["port_"+i],
        }
        global.Controllers.push(new Controller(settings));
    }
}




app.use(express.json())    // <==== parse request body as JSON
app.options('*', cors()) // include before other routes

app.use(upload.array()); 

app.use('/', cors(), require('./routes'));

app.listen(3005, () => {
    console.log("Serveur à l'écoute");
    loadControllerSettings();
});


setInterval(()=>{
    global.Controllers.forEach(async (c)=>{
        await c.open((chunck)=>{
            console.log(chunck);
        });

        await c.cmiReadVoltageAndStatus(0);
    });
}, 1000);