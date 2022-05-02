const express = require('express');
const router = express.Router();
const file = require('../file');

router.get('/', async (req, res, next) => {
    let obj = {};
    try{
        obj.controllers = [];
        for ( let i = 0 ; i < global.Controllers.length ; i++ ){

            let _c = global.Controllers[i];

            let totalModules = _c.totalBanks * _c.totalSeriesModules;

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