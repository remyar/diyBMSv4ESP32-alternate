const express = require('express');
const router = express.Router();
const file = require('../file');
const { SerialPort } = require('serialport');

router.get('/', async (req, res, next) => {
    try{
        const _ports = (await SerialPort.list()).filter((el) => el.friendlyName.includes("USB-SERIAL CH340"));

        let settings = await file.readSettings();

        settings.PortCom = [..._ports];
        
        res.json({settings : settings});
    }catch(err){
        console.error(err)
    }
});

module.exports = router;