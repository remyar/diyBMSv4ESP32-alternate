import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

function GlobalSettings(props) {

    const [BypassOverTempShutdown, setBypassOverTempShutdown] = useState(props.globalState.settings['BypassOverTempShutdown_' + props.idxController]);
    const [BypassThresholdmV, setBypassThresholdmV] = useState(props.globalState.settings['BypassThresholdmV_' + props.idxController]);

    return <div id="globalConfig" className="region" style={{width:'100%'}}>
        <h2>Global Settings Controller N° {props.idxController + 1}</h2>
        <p id="gc1">Configure all modules to use following parameters:</p>
        <div id="form">
            <div className="settings">
                <div>
                    <label for="g1">Bypass over temperature</label>
                    <input type="number" min="20" max="90" step="1" name="BypassOverTempShutdown" id="g1" value={BypassOverTempShutdown} required="" onChange={(event) => {
                        setBypassOverTempShutdown(event.target.value);
                    }} />
                </div>
                <div>
                    <label for="g2">Bypass threshold mV</label>
                    <input type="number" min="1800" max="4500" step="10" name="BypassThresholdmV" id="g2" value={BypassThresholdmV} required="" onChange={(event) => {
                        setBypassThresholdmV(event.target.value);
                    }} />
                </div>
            </div>
            <button style={{ cursor: "pointer" }} id="globalSettingsButton" onClick={() => {
                props.onClick && props.onClick(BypassOverTempShutdown , BypassThresholdmV);
            }}>Save settings</button>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(GlobalSettings)));