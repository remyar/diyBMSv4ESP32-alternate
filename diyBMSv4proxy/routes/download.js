const express = require('express');
const router = express.Router();
const path = require('path');

router.get('/', async (req, res, next) => {
    let query = req.query;
    try {
        let _p = path.resolve(__dirname, ".." + path.sep + "data" + path.sep + query.file);
        res.download(_p); // Set disposition and send it.
    } catch (err) {
        console.error(err)
    }
});


module.exports = router;