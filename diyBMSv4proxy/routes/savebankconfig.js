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
       
        await global.Controllers.forEach(async (c)=>{
            await c.close();
        });

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
        res.json({success : true});
    }catch(err){
        res.json({success : false});
    }
});

module.exports = router;