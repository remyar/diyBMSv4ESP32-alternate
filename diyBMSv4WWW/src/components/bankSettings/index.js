import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';
import { withSnackBar } from '../../providers/snackBar';

import actionsBms from '../../actions/bms';
import GlobalSettings from '../../components/globalSettings';

function BankSettings(props) {

    const defaultControlerSettings = {
        totalSeriesModules: 1,
        totalBanks: 1,
        baudrate: 9600
    }
    const settings = props.settings || {};

    let initiateState = [];
    for (let i = 0; i < settings.totalControllers; i++) {
        initiateState.push({
            totalSeriesModules: settings["totalNumberOfSeriesModules_" + i],
            totalBanks: settings["totalNumberOfBanks_" + i],
            baudrate: settings["baudRate_" + i],
            port: settings["port_" + i],
            BypassOverTempShutdown : settings["BypassOverTempShutdown_"+i],
            BypassThresholdmV : settings["BypassThresholdmV_"+i],
        })
    }
    const [totalControllers, setTotalControllers] = useState(initiateState);


    function _updateController(idx, val) {
        let _ctrl = [...totalControllers];
        _ctrl[idx] = { ..._ctrl[idx], ...val };
        setTotalControllers(_ctrl);
    }

    return <div>
        <h1>Controller Settings</h1>
        <div className="region">
            <h2 id="mb1">Modules &amp; Banks</h2>
            <p id="mb2">DIYBMS supports up to <span id='labelMaxModules'>128</span> modules in total. These modules can be
                split into banks to support parallel configurations.</p>
            <p id="mb3">Example: You have 16 cells configured as 8 in series and 2 in parallel (8S2P).</p>
            <p id="mb4">Only hardware module version 4.4 or newer support faster communication speeds.  You will need to reboot the controller manually if you change the speed.</p>

            <div id="form" >
                <div className="settings" >
                    <br />
                    {/*  <div >
                        <label for="totalControllers">Number of Total Controllers</label>
                        <select defaultValue={totalControllers.length} style={{ marginLeft: "10px", width: '50px' }} name="totalControllers" id="totalControllers" onChange={(event) => {
                            let _ctrl = [...totalControllers];
                            while (_ctrl.length < event.target.value) {
                                _ctrl.push({ ...defaultControlerSettings });
                            }
                            while (_ctrl.length > event.target.value) {
                                _ctrl.splice(_ctrl.length - 1, 1);
                            }
                            setTotalControllers(_ctrl);

                        }}>
                            {(() => {
                                let options = [];
                                for (let n = 1; n <= 4; n++) {
                                    options.push(<option>{n}</option>);
                                }
                                return options;
                            })()}
                        </select>
                        </div>*/}
                    {(() => {
                        let _retVal = [];
                        for (let n = 0; n < totalControllers.length; n++) {
                            _retVal.push(<div style={{ paddingTop: '15px' }}>
                                <h2 id="mb1">Controller N° {n + 1}</h2>
                                <div style={{ paddingTop: '5px' }}>
                                    <label >Number of series cells (eg. 8S)</label>
                                    <select defaultValue={totalControllers[n].totalSeriesModules} style={{ marginLeft: "10px", width: '100px' }} name="totalSeriesModules" id="totalSeriesModules" onChange={(event) => {
                                        _updateController(n, { totalSeriesModules: event.target.value });
                                    }}>
                                        {(() => {
                                            let options = [];
                                            for (let n = 1; n <= 128; n++) {
                                                options.push(<option>{n}</option>);
                                            }
                                            return options;
                                        })()}
                                    </select>
                                </div>
                                <div style={{ paddingTop: '5px' }}>
                                    <label>Number of parallel banks (eg. 2P)</label>
                                    <select defaultValue={totalControllers[n].totalBanks} style={{ marginLeft: "10px", width: '100px' }} name="totalBanks" id="totalBanks" onChange={(event) => {
                                        _updateController(n, { totalBanks: event.target.value });
                                    }}>
                                        {(() => {
                                            let options = [];
                                            for (let n = 1; n <= 16; n++) {
                                                options.push(<option>{n}</option>);
                                            }
                                            return options;
                                        })()}
                                    </select>
                                </div>
                                <div style={{ paddingTop: '5px' }}>
                                    <label >Communication speed</label>
                                    <select disabled defaultValue={totalControllers[n].baudrate} style={{ marginLeft: "10px" , width: '100px' }} name="baudrate" id="baudrate" onChange={(event) => {
                                        _updateController(n, { baudrate: event.target.value });
                                    }}>
                                        {(() => {
                                            let options = [];

                                            options.push(<option value="2400">Standard</option>);
                                            options.push(<option value="5000">5K</option>);
                                            options.push(<option value="9600">9K6</option>);
                                            return options;
                                        })()}
                                    </select>
                                </div>
                                <div style={{ paddingTop: '5px' }}>
                                    <label >Select Port</label>
                                    <select disabled defaultValue={totalControllers[n].port || settings.PortCom[0].path} style={{ marginLeft: "10px" , width: '100px'}} name="port" id="port" onChange={(event) => {
                                        _updateController(n, { port: event.target.value });
                                    }}>
                                        {(() => {
                                            return settings.PortCom.map((c) => <option value={c.path}>{c.path}</option>)
                                        })()}
                                    </select>
                                </div>
                                <div style={{ paddingTop: '5px' }}>
                                    <label for="g1">Bypass over temperature</label>
                                    <input style={{ marginLeft: "10px" , width: '100px' }} type="number" min="20" max="90" step="1" name="BypassOverTempShutdown" id="g1" defaultValue={totalControllers[n].BypassOverTempShutdown} required="" onChange={(event) => {
                                       // setBypassOverTempShutdown(event.target.value);
                                       _updateController(n, { BypassOverTempShutdown: event.target.value });
                                    }} />
                                </div>
                                <div style={{ paddingTop: '5px' }}>
                                    <label for="g2">Bypass threshold mV</label>
                                    <input style={{ marginLeft: "10px" , width: '100px' }} type="number" min="1800" max="4500" step="10" name="BypassThresholdmV" id="g2" defaultValue={totalControllers[n].BypassThresholdmV} required="" onChange={(event) => {
                                       // setBypassThresholdmV(event.target.value);
                                       _updateController(n, { BypassThresholdmV: event.target.value });
                                    }} />
                                </div>

                            </div>);
                        }
                        return _retVal;
                    })()}
                    <br />


                    <button style={{ cursor: "pointer" }} onClick={async () => {
                        let _obj = {};
                        _obj.totalControllers = totalControllers.length;

                        for (let _i = 0; _i < _obj.totalControllers; _i++) {
                            let _ctrl = totalControllers[_i];
                            _obj["totalSeriesModules_" + _i] = _ctrl.totalSeriesModules;
                            _obj["totalBanks_" + _i] = _ctrl.totalBanks;
                            _obj["BypassOverTempShutdown_" + _i] = _ctrl.BypassOverTempShutdown;
                            _obj["BypassThresholdmV_" + _i] = _ctrl.BypassThresholdmV;
                           // _obj["baudrate_" + _i] = _ctrl.baudrate;
                           // _obj["port_" + _i] = _ctrl.port || ((settings.PortCom) && (settings.PortCom[0] != undefined)) && settings.PortCom[0].path;
                        }

                        let response = await props.dispatch(actionsBms.setBankSettings(_obj));
                        if (response === true) {
                            props.snackbar.success("Saved");
                        } else {
                            props.snackbar.error("An Error was occured");
                        }
                    }}>Save module &amp; bank settings</button>
                </div>
            </div>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(withSnackBar(BankSettings))));