import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

function ModuleConfig(props) {
    const settings = props.globalState.modules[props.cellid] || {};

    const [BypassOverTempShutdown, setBypassOverTempShutdown] = useState(settings.BypassOverTempShutdown);
    const [BypassThresholdmV, setBypassThresholdmV] = useState(settings.BypassThresholdmV);
    const [Calib, setCalib] = useState(settings.Calib.toFixed(4));
    const [ActualVoltage, setActualVoltage] = useState(((props.globalState.monitor2?.voltages[props.cellid || 0] || 0) / 1000).toFixed(3));
    const [ExtBCoef, setExtBCoef] = useState(settings.ExtBCoef);
    const [IntBCoef, setIntBCoef] = useState(settings.IntBCoef);
    const [LoadRes, setILoadRes] = useState(settings.LoadRes.toFixed(2));
    const [mVPerADC, setmVPerADC] = useState(settings.mVPerADC.toFixed(2));

    return <div id="settingConfig">
        <h2>Settings for module </h2>
        <div id="form">
            <div className="settings">
                <input name="b" id="b" type="hidden" value="" />
                <input name="m" id="m" type="hidden" value={settings.id} />
                <input name="c" id="c" type="hidden" value={settings.id} />
                <div>
                    <label for="ModuleId">Module Id</label>
                    <input type="number" name="ModuleId" id="ModuleId" value={settings.id} readonly="" />
                </div>
                <div>
                    <label for="Version">Module version</label>
                    <span name="Version" id="Version" style={{paddingLeft : '5px'}}>{settings.ver.toString()}</span>
                </div>
                <div>
                    <label for="BypassOverTempShutdown">Bypass over temperature</label>
                    <input type="number" min="20" max="90" step="1" name="BypassOverTempShutdown" id="BypassOverTempShutdown" defaultValue={BypassOverTempShutdown} required=""
                        onChange={(event) => {
                            setBypassOverTempShutdown(event.target.value);
                        }} />
                </div>
                <div>
                    <label for="BypassThresholdmV">Bypass threshold mV</label>
                    <input type="number" min="1800" max="4500" step="10" name="BypassThresholdmV" id="BypassThresholdmV" defaultValue={BypassThresholdmV} required=""
                        onChange={(event) => {
                            setBypassThresholdmV(event.target.value);
                        }} />
                </div>
                <div>
                    <label for="Calib">Calibration multiplier</label>
                    <input id="Calib" name="Calib" type="number" min="1" max="5" step="0.0001" defaultValue={Calib} required="" />
                    <label for="ActualVoltage">Calculator - Actual measured voltage</label>
                    <input id="ActualVoltage" name="ActualVoltage" type="number" min="0" max="5" step="0.001"
                        defaultValue={ActualVoltage} />
                    <button type="button" id="CalculateCalibration">Calculate</button>
                </div>
                <div>
                    <label for="ExtBCoef">External temperature BCoef</label>
                    <input type="number" min="0" max="9999" step="1" id="ExtBCoef" name="ExtBCoef" value={ExtBCoef}
                        readonly="" />
                </div>
                <div>
                    <label for="IntBCoef">Internal temperature BCoef</label>
                    <input type="number" min="0" max="9999" step="1" id="IntBCoef" name="IntBCoef" value={IntBCoef}
                        readonly="" />
                </div>
                <div>
                    <label for="LoadRes">Load resistance</label>
                    <input id="LoadRes" name="LoadRes" type="number" value={LoadRes} readonly="" />
                </div>
                <div>
                    <label for="mVPerADC">mV per ADC reading</label>
                    <input id="mVPerADC" name="mVPerADC" type="number" value={mVPerADC} readonly="" />
                </div>

                <button style={{ cursor: 'pointer' }} onClick={() => {
                    props.dispatch(actionsBms.setModuleSettings({
                        b: "",
                        m: settings.id,
                        c: settings.id,
                        ModuleId: settings.id,
                        BypassOverTempShutdown: BypassOverTempShutdown,
                        BypassThresholdmV: BypassThresholdmV,
                        Calib : Calib,
                        ActualVoltage : ActualVoltage,
                        ExtBCoef : ExtBCoef,
                        IntBCoef : IntBCoef,
                        LoadRes : LoadRes,
                        mVPerADC : mVPerADC,
                    }));
                }}>Save settings</button>
            </div>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(ModuleConfig)));