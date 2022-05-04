const express = require('express');
const router = express.Router();
const file = require('../file');

router.get('/', async (req, res, next) => {
    let obj = {};
    try {
        obj.timenow = new Date().getHours() * 60 + new Date().getMinutes();
        for (let i = 0; i < global.Controllers.length; i++) {
            let _rules = await global.Controllers[i].controllerGetRules();
            obj["rules_" + i] = [];
            for (let j = 0; j < _rules.length; j++) {
                obj["rules_" + i][j] = {
                    value: _rules[j].rulevalue,
                    hysteresis: _rules[j].rulehysteresis,
                    relays: [..._rules[j].rulerelaystate]
                };
            }
        }

        for (let i = 0; i < global.Controllers.length; i++) {
            let _c = global.Controllers[i];
            for (let j = 0; j < _c.rule_outcome.length; j++) {
                obj["rules_" + i][j].triggered = parseInt(_c.rule_outcome[j]) ? true : false;
            }
        }

        for (let i = 0; i < global.Controllers.length; i++) {
            let _c = global.Controllers[i];
            obj["relaydefault_" + i] = [..._c.relayDefault];
        }

        res.json(obj);
    } catch (err) {
        res.json(obj);
    }
});


router.post('/', async (req, res, next) => {
    try {
        let _rules = {};
        for (let i = 0; i < global.Controllers.length; i++) {
            for (let rule = 0; rule < global.Controllers[i].RELAY_RULES; rule++) {

                let name = "rule_";
                name += i;
                name += "_";
                name += rule;
                name += "_value";

                if (_rules["rules_" + i] == undefined) {
                    _rules["rules_" + i] = [];
                }
                if (_rules["rules_" + i][rule] == undefined) {
                    _rules["rules_" + i][rule] = {};
                }
                _rules["rules_" + i][rule].value = req.body[name];
            }
            for (let rule = 0; rule < global.Controllers[i].RELAY_RULES; rule++) {
                let name = "rule_";
                name += i;
                name += "_";
                name += rule;
                name += "_hysteresis";

                _rules["rules_" + i][rule].hysteresis = req.body[name];
            }
            for (let rule = 0; rule < global.Controllers[i].RELAY_RULES; rule++) {
                for (let j = 0; j < global.Controllers[i].RELAY_TOTAL; j++) {
                    let name = "rule_";
                    name += i;
                    name += "_";
                    name += rule;
                    name += "_relay_";
                    name += (j + 1);
                    if (_rules["rules_" + i][rule].relays == undefined) {
                        _rules["rules_" + i][rule].relays = [];
                    }
                    _rules["rules_" + i][rule].relays[j] = req.body[name];
                }
            }

            for (let j = 0; j < global.Controllers[i].RELAY_TOTAL; j++) {
                let name = "relaydefault_";
                name += i;
                name += "_";
                name += j;
                if (_rules["relaydefault_" + i] == undefined) {
                    _rules["relaydefault_" + i] = [];
                }

                _rules['relaydefault_' + i][j] = req.body[name];
            }
        }

        for (let j = 0; j < global.Controllers.length; j++) {
            try {
                await global.Controllers[j].controllerSetRules({
                    timenow: new Date().getHours() * 60 + new Date().getMinutes(),
                    rules: _rules["rules_" + j],
                    relaydefault: _rules["relaydefault_" + j]
                });
            } catch (err) {
                console.error(err);
            }
        };
        res.json({ success: true });
    } catch (err) {
        res.json({ success: false });
    }
});

module.exports = router;