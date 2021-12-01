import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

function BankSettings(props) {

    const settings = props.settings || {};

    const [totalSeriesModules, setTotalSeriesModules] = useState(1);
    const [totalBanks, setTotalBanks] = useState(1);
    const [baudrate, setBaudrate] = useState(4100);

    return <div>
        <h1>Controller Settings</h1>
        <div className="region">
            <h2 id="mb1">Modules &amp; Banks</h2>
            <p id="mb2">DIYBMS supports up to <span id='labelMaxModules'>128</span> modules in total. These modules can be
                split into banks to support parallel configurations.</p>
            <p id="mb3">Example: You have 16 cells configured as 8 in series and 2 in parallel (8S2P).</p>
            <p id="mb4">Only hardware module version 4.4 or newer support faster communication speeds.  You will need to reboot the controller manually if you change the speed.</p>

            <div id="form" >
                <div className="settings">
                    <div>
                        <label for="totalSeriesModules">Number of series cells (eg. 8S)</label>
                        <select name="totalSeriesModules" id="totalSeriesModules" onChange={(event) => {
                            setTotalSeriesModules(event.target.value);
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
                    <div>
                        <label for="totalBanks">Number of parallel banks (eg. 2P)</label>
                        <select name="totalBanks" id="totalBanks" onChange={(event) => {
                            setTotalBanks(event.target.value);
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
                    <div>
                        <label for="baudrate">Communication speed</label>
                        <select name="baudrate" id="baudrate" defaultValue={settings.baudrate} onChange={(event) => {
                            setBaudrate(event.target.value);
                        }}>
                            {(() => {
                                let options = [];

                                options.push(<option value="2400">Standard</option>);

                                if (settings.supportSpeedChange) {
                                    options.push(<option value="5000">5K</option>);
                                    options.push(<option value="9600">9K6</option>);
                                }
                                return options;
                            })()}
                        </select>
                    </div>

                    <button style={{ cursor: "pointer" }} onClick={() => {
                        props.dispatch(actionsBms.setBankSettings({ totalSeriesModules, totalBanks, baudrate }));
                    }}>Save module &amp; bank settings</button>
                </div>
            </div>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(BankSettings)));