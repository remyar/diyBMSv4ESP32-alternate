const express = require('express');
const router = express.Router();
const file = require('../file');


router.get('/', async (req, res, next) => {
    try{
        console.log(req);
    }catch(err){

    }
});

router.post('/', async (req, res, next) => {
    try{
        let settings = await file.readGlobalSettings();
        settings['BypassOverTempShutdown_' + req.body.moduleId] = req.body.BypassOverTempShutdown;
        settings['BypassThresholdmV_' + req.body.moduleId] = req.body.BypassThresholdmV;
        await file.saveGlobalSettings(settings);

        global.Controllers[req.body.moduleId].BypassOverTempShutdown = req.body.BypassOverTempShutdown;
        global.Controllers[req.body.moduleId].BypassThresholdmV = req.body.BypassThresholdmV;


            try {
                await global.Controllers[req.body.moduleId].open();
                await global.Controllers[req.body.moduleId].controllerSetSettings();
            } catch (err) {
                console.error(err);
            }
 

        res.json({success : true});
    }catch(err){
        res.json({success : false});
    }
});

module.exports = router;