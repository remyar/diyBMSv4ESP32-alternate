const express = require('express');
const router = express.Router();
const file = require('../file');

router.get('/', async (req, res, next) => {
    let obj = {};
    let query = req.query;
    try {
        if ( global.Controllers.length > 0 && global.Controllers[query.c] != undefined){
            global.Controllers[query.c].controllerSetCmiIdentify(query.cmi);
        }
        res.json({ success: true });
    } catch (err) {
        console.error(err)
    }
});


module.exports = router;