const express = require('express');
const router = express.Router();
const file = require('../file');

router.get('/', async (req, res, next) => {
    let obj = {};
    try{


        res.json(obj);
    }catch(err){
        console.error(err)
    }
});


module.exports = router;