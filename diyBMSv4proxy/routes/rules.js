const express = require('express');
const router = express.Router();
const file = require('../file');

router.get('/', async (req, res, next) => {
    let obj = {};
    try {
        let _sett = await file.readRules();

        res.json(_sett);
    } catch (err) {
        console.error(err)
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

                for (let j = 0; j < 4; j++) {
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
            for (let j = 0; j < 4; j++) {
                let name = "relaydefault_";
                name += i;
                name += "_";
                name += j;

                _rules['relaydefault_' + i][j] = req.body[name];
            }
        }

        await file.saveRules(_rules);

        res.json({ success: true });
    } catch (err) {
        res.json({ success: false });
    }
});

module.exports = router;