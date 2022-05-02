const fs = require('fs');
const path = require('path');

module.exports = {
    process: async function (filename) {
        return new Promise((resolve, reject) => {
            let fileIdx = undefined;
            if (fs.existsSync(path.resolve(__dirname, "./data/" + filename)) == false) {
                fs.writeFileSync(path.resolve(__dirname, "./data/" + filename) , "DateTime," );
            }

            fs.appendFileSync(path.resolve(__dirname, "./data/" + filename) , "hello\r\n");
        })

    }
}