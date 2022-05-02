const express = require('express');
const router = express.Router();

router.use('/savebankconfig.json', require('./savebankconfig'));
router.use('/getbankconfig.json', require('./savebankconfig'));
router.use('/settings.json', require('./settings'));
router.use('/saveglobalsetting.json', require('./saveglobalsetting'));
router.use('/monitor2.json', require('./monitor2'));
router.use('/monitor3.json', require('./monitor3'));
router.use('/portcom.json', require('./portcom'));
router.use('/identifyModule.json', require('./identifyModule'));
router.use('/rules.json', require('./rules'));
router.use('/saverules.json', require('./rules'));
router.use('/storage.json', require('./storage'));
router.use('/savestorage.json', require('./storage'));

module.exports = router;