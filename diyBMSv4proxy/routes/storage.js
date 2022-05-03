const express = require('express');
const router = express.Router();
const file = require('../file');
const si = require('systeminformation');

router.get('/', async (req, res, next) => {
    let obj = { sdcard: { available: true } };
    try {
        let _sett = await file.readGlobalSettings();

        obj.logging = _sett.logging;
        obj.frequency = _sett.frequency;

        let _disk = await si.fsSize();
        let disk = _disk[0];

        obj.sdcard.used = disk.used / 1024;
        obj.sdcard.total = disk.size / 1024;

        obj.sdcard.files = await file.getFilesInDataDirectory();

        await file.saveGlobalSettings({ logging : _sett.logging , frequency : _sett.frequency});

        res.json(obj);
    } catch (err) {
        res.json(obj);
    }
});

router.post('/', async (req, res, next) => {
    try {
        let body = req.body;
        let _sett = {
            logging : body.loggingEnabled == "off" ? false : true,
            frequency : parseInt(body.loggingFreq) || 15
        }

        await file.saveGlobalSettings(_sett);

        global.loggingSettings = {..._sett};
        res.json({ success: true });
    } catch (err) {
        res.json({ success: false });
    }
});

module.exports = router;