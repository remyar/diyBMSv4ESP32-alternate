const express = require('express');
const router = express.Router();
const file = require('../file');

router.get('/', async (req, res, next) => {
    let obj = {};
    try{
        let settings = await file.readSettings();
        obj.controllers = [];

        for ( let i = 0 ; i < settings.totalControllers ; i++ ){

            let totalModules = settings["totalNumberOfBanks_" + i] * settings["totalNumberOfSeriesModules_" + i];

            let _ctrl = {
                badpacket : [],
                pktrecvd : [],
                balcurrent : []
            };

            for ( let j = 0 ; j < totalModules ; j++ ){
                _ctrl.badpacket.push(0);
                _ctrl.pktrecvd.push(0);
                _ctrl.balcurrent.push(0);
            }

            obj.controllers.push(_ctrl);
        }

        res.json(obj);
    }catch(err){
        console.error(err)
    }
});


module.exports = router;