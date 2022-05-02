const express = require('express');
const router = express.Router();
const file = require('../file');
const si = require('systeminformation');

router.get('/', async (req, res, next) => {
    let obj = { sdcard: {}, flash: {} , storage : {} };
    try {

        let _diskLayout = await si.fsSize();
        let diskLayout = _diskLayout[0];
        obj.sdcard.available = true;
        obj.sdcard.used = diskLayout.used / 1024 / 1024;
        obj.sdcard.total = diskLayout.size / 1024 /1024;
        
        obj.sdcard.files = await file.getDirectory();

        obj.logging = global.logging.loggingEnabled;
        obj.frequency = global.logging.loggingFrequencySeconds;

        res.json(obj);
    } catch (err) {
        console.error(err)
    }
});

router.post('/', async (req, res, next) => {

    try{
        global.logging = {
            loggingEnabled : req.body.loggingEnabled == 'on' ? true : false,
            loggingFrequencySeconds : parseInt(req.body.loggingFreq) || 15
        }
        await file.saveGlobalSettings(global.logging);
        res.json({ success: true });
    } catch (err) {
        res.json({ success: false });
    }
 
});

module.exports = router;