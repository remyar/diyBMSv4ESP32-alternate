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
                if ( _c.cmi[j].valid == true ){
                    _ctrl.balcurrent.push(_c.cmi[j].BalanceCurrentCount);
                    _ctrl.pktrecvd.push(_c.cmi[j].PacketReceivedCount);
                    _ctrl.badpacket.push(_c.cmi[j].badPacketCount);
                } else {
                    _ctrl.balcurrent.push("null");
                    _ctrl.badpacket.push("null");
                    _ctrl.pktrecvd.push("null");
                }
            }

            obj.controllers.push(_ctrl);
        }

        res.json(obj);
    }catch(err){
        console.error(err)
    }
});


module.exports = router;