const file = require('./file');
const path = require('path');
const fs = require('fs');

global.loggingSettings = {
    logging: false,
    frequency: 15
}

module.exports = {
    init: async function () {
        let _settings = await file.readGlobalSettings();
        global.loggingSettings = { ..._settings };
        let cpt = 0;
        setInterval(() => {
            cpt++;
            if ((cpt % global.loggingSettings.frequency) == 0) {
                if (global.loggingSettings.logging == true) {

                    for (let i = 0; i < global.Controllers.length; i++) {

                        let _d = new Date().toLocaleDateString('fr').replace("/", "_").replace("/", "_");
                        let _p = path.resolve(__dirname, '.' + path.sep + 'data' + path.sep);

                        let filename = "data_" + (i + 1) + "_" + _d + ".csv";
                        let _filePath = path.resolve(_p, filename);

                        if (fs.existsSync(_filePath) == false) {
                            let _str = "DateTime,";

                            let TotalNumberOfCells = global.Controllers[i].totalBanks * global.Controllers[i].totalSeriesModules;
                            for ( let j = 0 ; j < TotalNumberOfCells ; j++ ){
                                _str += "VoltagemV_" + j + ",";
                                _str += "ExternalTemp_" + j + ",";
                                _str += "Bypass_" + j + ",";
                                _str += "BalancemAh_" + j;

                                if ( j < (TotalNumberOfCells -1 ) ){
                                    _str += ",";
                                }else {
                                    _str +="\r\n";
                                }
                            }
                            fs.writeFileSync(_filePath, _str );
                        }

                        let _dataMessage = _d.replace("_","-").replace("_","-") + " " + new Date().toLocaleTimeString('fr') + ",";
                        let TotalNumberOfCells = global.Controllers[i].totalBanks * global.Controllers[i].totalSeriesModules;
                        for ( let j = 0 ; j < TotalNumberOfCells ; j++ ){

                            let cmi = global.Controllers[i].cmi;

                            _dataMessage += cmi[j].voltagemV + ",";
                            _dataMessage += cmi[j].externalTemp + ",";
                            _dataMessage += (cmi[j].inBypass ? 'Y' : 'N') + ",";
                            _dataMessage += cmi[j].BalanceCurrentCount;

                            if ( j < (TotalNumberOfCells -1 ) ){
                                _dataMessage += ",";
                            }else {
                                _dataMessage +="\r\n";
                            }

                        }
                        fs.appendFileSync(_filePath, _dataMessage);
                    }
                }
            }
        }, 1000)
    }
}