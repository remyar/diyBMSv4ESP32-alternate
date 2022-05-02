const express = require('express');
const router = express.Router();
const file = require('../file');
const Controller = require('../controller');

router.get('/', async (req, res, next) => {
    try {
        console.log(req);
    } catch (err) {

    }
});

router.post('/', async (req, res, next) => {
    try {
        let _sett = req.body;

        for (let i = 0; i < global.Controllers.length ; i++) {
            await global.Controllers[i].controllerSetSettings({ 
                totalBanks: _sett["totalBanks_" + i], 
                totalSeriesModules: _sett["totalSeriesModules_" + i], 
                BypassOverTempShutdown: _sett['BypassOverTempShutdown_' + i], 
                BypassThresholdmV: _sett['BypassThresholdmV_' + i] 
            });
        }
        res.json({ success: true });
    } catch (err) {
        res.json({ success: false });
    }
});

module.exports = router;