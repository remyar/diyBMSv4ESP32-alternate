import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';
import localeData from '../../locales';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

import BankSettings from '../../components/bankSettings';
import ResetCounter from '../../components/resetCounter';
import RestartController from '../../components/restartController';
import NtpSettings from '../../components/ntpSettings';
import { intlFormat } from 'date-fns';

function Settings(props) {

    const settings = props.globalState.settings || {};

    const [Language, setLanguage] = useState(document.documentElement.lang);
    const [VoltageHigh, setVoltageHigh] = useState(settings.VoltageHigh);
    const [VoltageLow, setVoltageLow] = useState(settings.VoltageLow);

    return <div>

        <BankSettings settings={settings} />

        {/*
        <NtpSettings settings />

        <div className="region">
            <h2>Display Settings</h2>
            <div id="form">
                <div className="settings">
                    <div>
                        <label for="Language">Language</label>
                        <select name="Language" id="Language" defaultValue={Language} onChange={(event) => {
                            setLanguage(event.target.value);
                        }}>
                            <option value="en">English</option>
                            <option value="es">Spanish</option>
                            <option value="pt">Portuguese</option>
                            <option value="de">German</option>
                            <option value="nl">Dutch</option>
                            <option value="hr">Croatian</option>
                            <option value="ru">Russian</option>
                        </select>
                    </div>
                </div>

                <div>
                    <label for="VoltageHigh">Graph voltage scale (high)</label>
                    <select name="VoltageHigh" id="VoltageHigh" defaultValue={VoltageHigh} onChange={(event) => {
                        setVoltageHigh(event.target.value);
                    }}>
                        <option>4.50</option>
                        <option>4.25</option>
                        <option>4.00</option>
                        <option>3.75</option>
                        <option>3.50</option>
                        <option>3.25</option>
                        <option>3.00</option>
                        <option>2.75</option>
                        <option>2.50</option>
                        <option>2.25</option>
                        <option>2.00</option>
                    </select>
                </div>

                <div>
                    <label for="VoltageLow">Graph voltage scale (low)</label>
                    <select name="VoltageLow" id="VoltageLow" defaultValue={VoltageLow} onChange={(event) => {
                        setVoltageLow(event.target.value);
                    }}>
                        <option>4.50</option>
                        <option>4.25</option>
                        <option>4.00</option>
                        <option>3.75</option>
                        <option>3.50</option>
                        <option>3.25</option>
                        <option>3.00</option>
                        <option>2.75</option>
                        <option>2.50</option>
                        <option>2.25</option>
                        <option>1.75</option>
                        <option>1.50</option>
                        <option>1.25</option>
                        <option>1.00</option>
                        <option>0.50</option>
                        <option>0.00</option>
                    </select>
                </div>

                <button style={{ sursor: 'pointer' }} onClick={() => {
                    props.dispatch(actionsBms.setDisplaySettings({
                        Language,
                        VoltageHigh,
                        VoltageLow
                    })).then(()=>{
                        document.documentElement.lang = Language;
                    })
                }}>Save display settings</button>
            </div>
        </div>

        <ResetCounter settings />

        <RestartController settings />
            */}
    </div>;
}


export default withStoreProvider(withNavigation(injectIntl(Settings)));