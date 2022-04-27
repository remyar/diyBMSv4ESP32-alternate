const express = require('express');
const router = express.Router();
const file = require('../file');


router.get('/', async (req, res, next) => {
    try{
        console.log(req);
    }catch(err){

    }
});

router.post('/', async (req, res, next) => {
    try{
        await file.saveGlobalSettings(req.body);
        res.json({success : true});
    }catch(err){
        res.json({success : false});
    }
});

module.exports = router;