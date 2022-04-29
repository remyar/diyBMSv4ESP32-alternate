const express = require('express');
const router = express.Router();
const file = require('../file');

router.get('/', async (req, res, next) => {
    let obj = {};
    try {
        let _sett = await file.readRules();
        _sett.timenow = new Date().getHours() * 60 + new Date().getMinutes();

        for ( let i = 0 ; i < global.Controllers.length ; i++ ){
            let _c = global.Controllers[i];
            for ( let j = 0 ; j < _c.rule_outcome.length ; j++){
                _sett["rules_" + i][j].triggered = parseInt(_c.rule_outcome[j]) ? true : false;
            }
        }

        res.json(_sett);
    } catch (err) {
        res.json(obj);
    }
});


router.post('/', async (req, res, next) => {
    try {
        let _rules = await file.readRules();
        let _sett = await file.readBankConfiguration();
        for (let i = 0; i < parseInt(_sett.totalControllers); i++) {
            for (let rule = 0; rule < 8; rule++) {

                let name = "rule_";
                name += i;
                name += "_";
                name += rule;
                name += "_value";

                _rules["rules_" + i][rule].value = req.body[name];
            }
        }

        for (let i = 0; i < parseInt(_sett.totalControllers); i++) {
            for (let rule = 0; rule < 8; rule++) {

                let name = "rule_";
                name += i;
                name += "_";
                name += rule;
                name += "_hysteresis";

                _rules["rules_" + i][rule].hysteresis = req.body[name];
            }
        }

        for (let i = 0; i < parseInt(_sett.totalControllers); i++) {
            for (let rule = 0; rule < 8; rule++) {

                for (let j = 0; j < 3; j++) {
                    let name = "rule_";
                    name += i;
                    name += "_";
                    name += rule;
                    name += "_relay_";
                    name += (j + 1);

                    _rules["rules_" + i][rule].relays[j] = req.body[name];
                }
            }
        }

        for (let i = 0; i < parseInt(_sett.totalControllers); i++) {
            for (let j = 0; j < 3; j++) {
                let name = "relaydefault_";
                name += i;
                name += "_";
                name += j;

                _rules['relaydefault_' + i][j] = req.body[name];
            }
        }
        
        for (let j = 0; j < global.Controllers.length; j++) {
            try {
                await global.Controllers[j].open();
                await global.Controllers[j].controllerSetRules({
                    timenow: new Date().getHours() * 60 + new Date().getMinutes(),
                    rules: _rules["rules_" + j],
                    relaydefault: _rules["relaydefault_" + j]
                });
            } catch (err) {
                console.error(err);
            }
        };

        await file.saveRules(_rules);

        res.json({ success: true });
    } catch (err) {
        res.json({ success: false });
    }
});

module.exports = router;