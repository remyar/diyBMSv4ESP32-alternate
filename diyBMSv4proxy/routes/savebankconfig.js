const express = require('express');
const router = express.Router();
const file = require('../file');
const Controller = require('../controller');

router.get('/', async (req, res, next) => {
    try{
        console.log(req);
    }catch(err){

    }
});

router.post('/', async (req, res, next) => {
    try{
        await file.saveBankConfiguration(req.body);
        let _sett = await file.readBankConfiguration();
        let globalSettings =  await file.readGlobalSettings();
        for ( let i = 0 ; i < global.Controllers.length ; i++){
            await global.Controllers[i].close();
        }

        global.Controllers = [];
        
        for ( let i = 0 ; i < parseInt(_sett.totalControllers) ; i++){
            let settings = {
                baudrate : _sett["baudrate_"+i],
                totalBanks : _sett["totalBanks_"+i],
                totalSeriesModules : _sett["totalSeriesModules_"+i],
                port : _sett["port_"+i],
                BypassOverTempShutdown : globalSettings['BypassOverTempShutdown_' + i ],
                BypassThresholdmV : globalSettings['BypassThresholdmV_'+i]
            }
            global.Controllers.push(new Controller(settings));

        }

        for ( let j = 0 ; j < global.Controllers.length ; j++){
            try {
                await global.Controllers[j].open();
                await global.Controllers[j].controllerSetSettings();
            } catch (err) {
                console.error(err);
            }
        };
        
        res.json({success : true});
    }catch(err){
        res.json({success : false});
    }
});

module.exports = router;