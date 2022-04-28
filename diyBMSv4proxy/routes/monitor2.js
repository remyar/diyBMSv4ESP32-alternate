const express = require('express');
const router = express.Router();
const file = require('../file');

router.get('/', async (req, res, next) => {
    let obj = {};
    try {
        let settings = await file.readSettings();
         obj.controllers = [];

        for (let i = 0; i < settings.totalControllers; i++) {

            let totalModules = settings["totalNumberOfBanks_" + i] * settings["totalNumberOfSeriesModules_" + i];

            let _ctrl = {
                banks: settings["totalNumberOfBanks_" + i],
                seriesmodules: settings["totalNumberOfSeriesModules_" + i],
                errors: [],
                voltages: [],
                minvoltages: [],
                maxvoltages: [],
                inttemp: [],
                exttemp: [],
                bypass: [],
                bypasspwm: [],
                bypasshot: [],
            };

            for (let j = 0; j < totalModules; j++) {

                global.Controllers.forEach(async (c) => {
                    let cmi = c.cmi[j];
                    if ( cmi ){
                        _ctrl.voltages.push(cmi.valid ? cmi.voltagemV : null);
                        _ctrl.minvoltages.push(cmi.valid ? cmi.voltagemVMin : null);
                        _ctrl.maxvoltages.push(cmi.valid ? cmi.voltagemVMax : null);
                        _ctrl.inttemp.push(cmi.valid ? cmi.internalTemp : null);
                        _ctrl.exttemp.push(cmi.valid ? cmi.externalTemp : null);
                        _ctrl.bypass.push(cmi.valid ? cmi.inBypass : null);
                        _ctrl.bypasspwm.push(cmi.valid ? cmi.PWMValue : null);
                        _ctrl.bypasshot.push(cmi.valid ? cmi.PWMValue : null);
                    }
                });


            }

            obj.controllers.push(_ctrl);
        }

        res.json(obj);
    } catch (err) {
        console.error(err)
    }
});


module.exports = router;