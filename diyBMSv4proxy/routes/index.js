const express = require('express');
const router = express.Router();

router.use('/savebankconfig.json', require('./savebankconfig'));
router.use('/settings.json', require('./settings'));
router.use('/saveglobalsetting.json', require('./saveglobalsetting'));
router.use('/monitor2.json', require('./monitor2'));
router.use('/monitor3.json', require('./monitor3'));
router.use('/portcom.json', require('./portcom'));

module.exports = router;